// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Hook.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "CableComponent.h"
#include "Data/HookData.h"
#include "MISC/AsyncLoad.h"

AHook::AHook()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	HookMesh->SetupAttachment(RootComponent);
	HookMesh->CastShadow = false;
	HookMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HookMesh->SetRelativeRotation(FRotator{ -90.0f, 0.0f, 0.0f });

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(HookMesh, EndPointSocket);
	Cable->SetCollisionProfileName(TEXT("NoCollision"));
	Cable->bSkipCableUpdateWhenNotVisible = true;
	Cable->EndLocation = FVector::ZeroVector;
	Cable->CableLength = 0.0f;
	Cable->CastShadow = false;
}

void AHook::Initialize(const FHookData& Data, bool bLoadAsync)
{
	const auto MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	LoadAssets(Data, bLoadAsync);

	HandSocket = Data.HandSocket;

	Speed = Data.Speed;
	Distance = Data.Distance;
	BoostPower = Data.BoostPower;
	MaxBoostPower = Data.MaxBoostPower;
	MaxMoveDuration = Data.MaxMoveDuration;
	PenetrationOffset = Data.PenetrationOffset;
	EndMoveLaunchPower = Data.EndMoveLaunchPower;

	const FTransform DefaultTransform = GetClass()
		->GetDefaultObject<AHook>()->HookMesh->GetRelativeTransform();

	FTransform HookTransform = Data.HookTransform;
	HookTransform *= DefaultTransform;
	HookMesh->SetRelativeTransform(HookTransform);

	Cable->CableWidth = Data.Thickness;
	Cable->SetAttachEndToComponent(MyOwner->GetMesh(), HandSocket);
}

void AHook::Hook()
{
	if (State != EHookState::Idle) return;

	TraceHookTarget();

	StartLoc = GetHandLoc();
	const FRotator Rot = FRotationMatrix::
		MakeFromX(HookLoc - StartLoc).Rotator();
	
	SetActorLocationAndRotation(StartLoc, Rot);
	MulticastSetVisibility(true);
	
	State = EHookState::Throw;
}

void AHook::Unhook()
{
	if (State == EHookState::Idle) return;
	
	const auto MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	UE_LOG(LogTemp, Log, TEXT("%s"), *MyOwner->GetVelocity().ToString());
	MyOwner->LaunchCharacter(MyOwner->GetVelocity(), true, true);
	Clear();
}

void AHook::MoveTo()
{
	if (State != EHookState::Swing) return;
	
	const auto MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	MyOwner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	StartLoc = MyOwner->GetActorLocation();
	State = EHookState::Move;
}

void AHook::PostActorCreated()
{
	Super::PostActorCreated();

	GetClass()->GetDefaultObject<AHook>()->ApplyProperty();
	ApplyProperty();
}

#if WITH_EDITOR

void AHook::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ApplyProperty();
}

#endif

void AHook::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
		Clear();
}

void AHook::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!GetOwner()->HasAuthority())
		return;

	switch (State)
	{
	case EHookState::Throw:
		TickThrow(DeltaSeconds);
		break;
	case EHookState::Swing:
		TickSwing(DeltaSeconds);
		break;
	case EHookState::Move:
		TickMove(DeltaSeconds);
		break;
	}
}

void AHook::GetLifetimeReplicatedProps(TArray
	<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHook, Length);
}

void AHook::MulticastSetVisibility_Implementation(bool bNewVisibility)
{
	HookMesh->SetVisibility(bNewVisibility);
	Cable->SetVisibility(bNewVisibility);
}

void AHook::OnRep_Length()
{
	Cable->CableLength = Length;
}

void AHook::TickThrow(float DeltaSeconds)
{
	const float Dist = FVector::Distance(StartLoc, HookLoc);
	const FVector NewLoc = FMath::Lerp(GetActorLocation(),
		HookLoc + GetOffset(), FMath::Min(DeltaSeconds * Speed, 1.0f));
	
	SetActorLocation(NewLoc);

	const float TargetDist = FVector::DistSquared(NewLoc, HookLoc);
	const float HookDist = FVector::DistSquared(StartLoc, NewLoc);

	if (TargetDist < HookTolerance * HookTolerance)
		EndThrow(static_cast<bool>(HookedTarget));
	else if (HookDist > Distance * Distance)
		EndThrow(false);
}

void AHook::TickSwing(float DeltaSeconds)
{
	const auto* MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	float ForceCoef = FVector::Dist2D(GetHandLoc(), HookLoc) / Length;
	ForceCoef = 1.0f - ForceCoef;
	ForceCoef = FMath::Clamp(ForceCoef, 0.0f, 1.0f);

	const FVector OwnerLoc = MyOwner->GetActorLocation();
	const FVector OwnerVelocity = MyOwner->GetVelocity();

	FVector Force; float Velocity;

	if (OwnerLoc.Z < HookLoc.Z)
	{
		Force = OwnerLoc - HookLoc;
		Velocity = OwnerVelocity | Force;
		Force.Normalize();

		MyOwner->GetCharacterMovement()->
			AddForce(Force * Velocity * -2.0f * ForceCoef);

		Velocity = OwnerVelocity.GetSafeNormal()
			| MyOwner->GetControlRotation().Vector();

		Velocity = ForceCoef * BoostPower * FMath::Clamp(Velocity, 0.0f, 1.0f);
		Force = (OwnerVelocity * Velocity).GetClampedToMaxSize(MaxBoostPower);
		MyOwner->GetCharacterMovement()->AddForce(Force);
	}

	Velocity = FMath::Max(FVector::Distance
		(OwnerLoc, HookLoc) - Length, 0.0f) * 400.0f;
	
	Force = FRotationMatrix::MakeFromX(HookLoc - OwnerLoc).Rotator().Vector();
	MyOwner->GetCharacterMovement()->AddImpulse(Force * Velocity);
}

void AHook::TickMove(float DeltaSeconds)
{
	const auto MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	const FVector TargetLoc = HookLoc + GetOffset();
	const bool bComplete = FVector::DistSquared(MyOwner->
		GetActorLocation(), TargetLoc) <= MoveTolerance * MoveTolerance;

	if (bComplete)
	{
		Clear();
		return;
	}

	TimeElapsed += DeltaSeconds;

	const float BlendPct = TimeElapsed / MaxMoveDuration;
	const FVector NewLoc = bComplete ? TargetLoc : FMath::Lerp(StartLoc, TargetLoc, BlendPct);
	MyOwner->SetActorLocation(NewLoc);
}

void AHook::EndThrow(bool bSuccess)
{
	if (!bSuccess)
	{
		Clear();
		return;
	}

	const FVector FinalLoc = HookLoc + GetOffset();
	SetActorLocationAndRotation(FinalLoc, HookRot);
	AddActorLocalRotation(FRotator{ 180.0f, 0.0f, 0.0f });
	AddActorLocalOffset(FVector{ PenetrationOffset, 0.0f, 0.0f });
	AttachToComponent(HookedTarget, FAttachmentTransformRules::KeepWorldTransform);

	Cable->CableLength = Length = FVector::Distance(GetHandLoc(), FinalLoc);
	State = EHookState::Swing;
}

void AHook::LoadAssets(const FHookData& Data, bool bLoadAsync)
{
	if (bLoadAsync)
	{
		AsyncLoad(Data.HookMesh, [HookMesh = HookMesh](const auto& Ptr)
			{
				HookMesh->SetStaticMesh(Ptr.Get());
			});

		AsyncLoad(Data.CableMaterial, [Cable = Cable](const auto& Ptr)
			{
				Cable->SetMaterial(0, Ptr.Get());
			});

		AsyncLoad(Data.ThrowSound, [this](const auto& Ptr)
			{
				ThrowSound = Ptr.Get();
			});

		AsyncLoad(Data.PullSound, [this](const auto& Ptr)
			{
				PullSound = Ptr.Get();
			});
	}
	else
	{
		HookMesh->SetStaticMesh(Data.HookMesh.LoadSynchronous());
		Cable->SetMaterial(0, Data.CableMaterial.LoadSynchronous());
		ThrowSound = Data.ThrowSound.LoadSynchronous();
		PullSound = Data.PullSound.LoadSynchronous();
	}
}

void AHook::TraceHookTarget()
{
	FVector Start; FRotator Dir;
	GetOwner()->GetActorEyesViewPoint(Start, Dir);

	HookLoc = Start + (Dir.Vector() * Distance);

	FHitResult Result;
	if (GetWorld()->LineTraceSingleByProfile
		(Result, Start, HookLoc, CollisionProfile))
	{
		HookedTarget = Result.Component.Get();
		FirstHookLoc = HookedTarget->GetComponentLocation();
		HookLoc = Result.Location;
		HookRot = FRotationMatrix::MakeFromX(Result.Normal).Rotator();
	}
}

void AHook::ApplyProperty()
{
	Cable->AttachToComponent(HookMesh,
		FAttachmentTransformRules::KeepRelativeTransform, EndPointSocket);
}

void AHook::Clear()
{
	const auto MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	if (State == EHookState::Move)
	{
		const FVector Dir = MyOwner->GetActorLocation() - StartLoc;
		MyOwner->LaunchCharacter(Dir * EndMoveLaunchPower, true, true);
	}

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	MyOwner->GetCharacterMovement()->SetDefaultMovementMode();
	MulticastSetVisibility(false);

	HookedTarget = nullptr;
	FirstHookLoc = HookLoc = StartLoc = FVector::ZeroVector;
	HookRot = FRotator::ZeroRotator;
	Length = TimeElapsed = 0.0f;

	State = EHookState::Idle;
}

FVector AHook::GetHandLoc() const
{
	const auto* MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	return MyOwner->GetMesh()->GetSocketLocation(HandSocket);
}

FVector AHook::GetOffset() const
{
	if (!HookedTarget) return FVector::ZeroVector;
	return HookedTarget->GetComponentLocation() - FirstHookLoc;
}
