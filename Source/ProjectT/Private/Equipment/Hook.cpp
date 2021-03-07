// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Hook.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "CableComponent.h"
#include "Data/HookData.h"
#include "MISC/AsyncLoad.h"

AHook::AHook()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	HookMesh->SetupAttachment(RootComponent);
	HookMesh->CastShadow = false;
	HookMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HookMesh->SetRelativeRotation(FRotator{ -90.0f, 0.0f, 0.0f });

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(HookMesh, EndPointSocket);
	Cable->SetCollisionProfileName(TEXT("NoCollision"));
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
	HookTolerance = Data.HookTolerance;
	MaxMoveDuration = Data.MaxMoveDuration;
	PenetrationOffset = Data.PenetrationOffset;

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

	const FRotator Rot = FRotationMatrix::
		MakeFromX(HookLoc - StartLoc).Rotator();

	SetActorRotation(Rot);

	HookMesh->SetVisibility(true);
	Cable->SetVisibility(true);

	State = EHookState::Throw;
}

void AHook::Unhook()
{
	if (State == EHookState::Throw || State == EHookState::Swing)
		Clear();
}

void AHook::MoveTo()
{
	if (State == EHookState::Swing)
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

	HookMesh->SetVisibility(false);
	Cable->SetVisibility(false);
}

void AHook::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!GetOwner()->HasAuthority())
		return;

	switch (State)
	{
	case EHookState::Idle:
		TickIdle(DeltaSeconds);
		break;
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

void AHook::GetLifetimeReplicatedProps
	(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AHook::TickIdle(float DeltaSeconds)
{
	const auto* MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	StartLoc = MyOwner->GetMesh()->
		GetSocketLocation(HandSocket);

	SetActorLocation(StartLoc);
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

}

void AHook::TickMove(float DeltaSeconds)
{

}

void AHook::EndThrow(bool bSuccess)
{
	if (!bSuccess)
	{
		Clear();
		return;
	}

	SetActorLocationAndRotation(HookLoc + GetOffset(), HookRot);
	AddActorLocalRotation(FRotator{ 180.0f, 0.0f, 0.0f });
	AddActorLocalOffset(FVector{ PenetrationOffset, 0.0f, 0.0f });

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
	HookMesh->SetVisibility(false);
	Cable->SetVisibility(false);

	HookedTarget = nullptr;
	FirstHookLoc = HookLoc = StartLoc = FVector::ZeroVector;
	HookRot = FRotator::ZeroRotator;

	State = EHookState::Idle;
}

FVector AHook::GetOffset() const noexcept
{
	if (!HookedTarget) return FVector::ZeroVector;
	return HookedTarget->GetComponentLocation() - FirstHookLoc;
}
