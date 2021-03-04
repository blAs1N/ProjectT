// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Hook.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CableComponent.h"
#include "Data/HookData.h"
#include "MISC/AsyncLoad.h"

AHook::AHook()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	HookMesh->SetupAttachment(RootComp);
	HookMesh->SetCollisionProfileName(TEXT("NoCollision"));

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(RootComp);
	Cable->SetCollisionProfileName(TEXT("NoCollision"));
	Cable->EndLocation = FVector::ZeroVector;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	MovementComp->bAutoActivate = false;
	MovementComp->bIsHomingProjectile = true;
	MovementComp->ProjectileGravityScale = 0.0f;
}

void AHook::Initialize(const FHookData& Data, bool bLoadAsync)
{
	const auto MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	LoadAssets(Data, bLoadAsync);

	HookMesh->SetRelativeTransform(Data.HookTransform);

	Cable->CableWidth = Data.Thickness;
	Cable->SetAttachEndToComponent(MyOwner->GetMesh(), Data.HandSocket);
	AttachToComponent(MyOwner->GetMesh(), FAttachmentTransformRules
		::SnapToTargetNotIncludingScale, Data.HandSocket);

	MovementComp->InitialSpeed = MovementComp->MaxSpeed
		= MovementComp->HomingAccelerationMagnitude = Data.Speed;

	Distance = Data.Distance;
	MaxMoveDuration = Data.MaxMoveDuration;
	PenetrationOffset = Data.PenetrationOffset;
}

void AHook::Hook()
{
	if (State != EHookState::Idle) return;

	TraceHookTarget();

	HookMesh->SetVisibility(true);
	Cable->SetVisibility(true);
	MovementComp->Activate();

	State = EHookState::Throw;
}

void AHook::Unhook()
{
	if (State == EHookState::Hook)
		CleanVariable();
}

void AHook::MoveTo()
{
	if (State == EHookState::Hook)
		State = EHookState::Move;
}

void AHook::BeginPlay()
{
	Super::BeginPlay();

	HookMesh->SetVisibility(false);
	Cable->SetVisibility(false);
}

void AHook::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AHook::GetLifetimeReplicatedProps
	(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

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

	HookLocation = Start + (Dir.Vector() * Distance);

	FHitResult Result;
	if (GetWorld()->LineTraceSingleByProfile
		(Result, Start, HookLocation, CollisionProfile))
	{
		HookedTarget = Result.Component.Get();
		MovementComp->HomingTargetComponent = Result.Component;

		FirstHookLocation = HookedTarget->GetComponentLocation();
		HookLocation = Result.Location;
		HookNormal = Result.Normal;
	}
}

void AHook::CleanVariable()
{
	HookMesh->SetVisibility(false);
	Cable->SetVisibility(false);

	HookMesh->SetRelativeLocationAndRotation
		(FVector::ZeroVector, FQuat::Identity);

	HookedTarget = nullptr;
	FirstHookLocation = HookLocation
		= HookNormal = FVector::ZeroVector;

	State = EHookState::Idle;
}
