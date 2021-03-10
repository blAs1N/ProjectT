// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Hook.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "CableComponent.h"
#include "Data/HookData.h"
#include "Equipment/State/IdleState.h"
#include "Equipment/State/MoveState.h"
#include "Equipment/State/SwingState.h"
#include "Equipment/State/ThrowState.h"
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
	LoadAssets(Data, bLoadAsync);
	HandSocket = Data.HandSocket;
	Stat = Data.Stat;

	const FTransform DefaultTransform = GetClass()
		->GetDefaultObject<AHook>()->HookMesh->GetRelativeTransform();

	FTransform HookTransform = Data.HookTransform;
	HookTransform *= DefaultTransform;
	HookMesh->SetRelativeTransform(HookTransform);
	Cable->CableWidth = Data.Thickness;

	if (GetOwner())
		OnRep_Owner();
}

void AHook::Hook()
{
	if (State == EHookState::Idle)
		SetState(EHookState::Throw);
}

void AHook::Unhook()
{
	if (State != EHookState::Idle)
		SetState(EHookState::Idle);
}

void AHook::MoveTo()
{
	if (State == EHookState::Swing)
		SetState(EHookState::Move);
}

void AHook::TraceHookTarget()
{
	FVector Start;
	GetOwner()->GetActorEyesViewPoint(Start, HookRot);
	HookLoc = Start + (HookRot.Vector() * Stat.Distance);

	FHitResult Result;
	if (GetWorld()->LineTraceSingleByProfile
		(Result, Start, HookLoc, CollisionProfile))
	{
		HookedTarget = Result.Component.Get();
		FirstTargetLoc = HookedTarget->GetComponentLocation();

		HookLoc = Result.Location;
		HookRot = FRotationMatrix::MakeFromX(Result.Normal).Rotator();
	}
	else
	{
		HookedTarget = nullptr;
		FirstTargetLoc = FVector::ZeroVector;
		HookRot += FRotator{ 0.0f, 180.0f, 0.0f };
	}
}

void AHook::SetState(EHookState NewState)
{
	if (!GetOwner()) return;
	
	States[static_cast<uint8>(State)]->Exit();
	States[static_cast<uint8>(NewState)]->Enter();
	State = NewState;
}

FVector AHook::GetHookLocation() const
{
	if (!HookedTarget) return HookLoc;

	return HookLoc + HookedTarget->GetComponentLocation() - FirstTargetLoc;
}

FVector AHook::GetHandLocation() const
{
	return CastChecked<ACharacter>(GetOwner())
		->GetMesh()->GetSocketLocation(HandSocket);
}

void AHook::BeginPlay()
{
	Super::BeginPlay();

	HookMesh->SetVisibility(false);
	Cable->SetVisibility(false);

	Cable->AttachToComponent(HookMesh,
		FAttachmentTransformRules::KeepRelativeTransform, EndPointSocket);

	if (GetOwner() && GetOwner()->HasAuthority())
		AllocateState();
}

void AHook::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	if (!NewOwner) return;

	AllocateState();
	OnRep_Owner();
}

void AHook::OnRep_Owner()
{
	Cable->SetAttachEndToComponent(GetOwner
		<ACharacter>()->GetMesh(), HandSocket);
}

void AHook::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetOwner() && GetOwner()->HasAuthority())
		States[static_cast<uint8>(State)]->Tick(DeltaSeconds);
}

void AHook::MulticastSetVisibility_Implementation(bool bNewVisibility)
{
	HookMesh->SetVisibility(bNewVisibility);
	Cable->SetVisibility(bNewVisibility);
}

void AHook::MulticastSetLength_Implementation(float NewLength)
{
	Cable->CableLength = Length = NewLength;
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
	}
	else
	{
		HookMesh->SetStaticMesh(Data.HookMesh.LoadSynchronous());
		Cable->SetMaterial(0, Data.CableMaterial.LoadSynchronous());
	}
}

void AHook::AllocateState()
{
	if (States.Num() > 0) return;
	
	States.Emplace(new FIdleState{});
	States[0]->SetOwner(this);

	States.Emplace(new FThrowState{});
	States[1]->SetOwner(this);

	States.Emplace(new FSwingState{});
	States[2]->SetOwner(this);

	States.Emplace(new FMoveState{});
	States[3]->SetOwner(this);
}
