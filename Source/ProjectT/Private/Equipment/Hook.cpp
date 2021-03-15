// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Hook.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "CableComponent.h"
#include "Data/Hook/HookContextParam.h"
#include "Data/Hook/HookData.h"
#include "Equipment/State/IdleState.h"
#include "Equipment/State/MoveState.h"
#include "Equipment/State/SwingState.h"
#include "Equipment/State/ThrowState.h"
#include "Equipment/HookContext.h"
#include "MISC/AsyncLoad.h"

AHook::AHook()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicateMovement(true);
	bReplicates = true;

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
	bInit = true;

	const FTransform DefaultTransform = GetClass()
		->GetDefaultObject<AHook>()->HookMesh->GetRelativeTransform();

	FTransform HookTransform = Data.HookTransform;
	HookTransform *= DefaultTransform;
	HookMesh->SetRelativeTransform(HookTransform);
	Cable->CableWidth = Data.Thickness;

	if (const auto MyOwner = GetOwner<ACharacter>())
		Cable->SetAttachEndToComponent(MyOwner->GetMesh(), HandSocket);

	if (Context) OnRep_Context();
}

void AHook::Hook()
{
	if (GetOwner() && State == EHookState::Idle)
		SetState(EHookState::Throw);
}

void AHook::Unhook()
{
	if (GetOwner() && State != EHookState::Idle)
		SetState(EHookState::Idle);
}

void AHook::MoveTo()
{
	if (GetOwner() && State == EHookState::Swing)
		SetState(EHookState::Move);
}

void AHook::SetState(EHookState NewState)
{
	States[static_cast<uint8>(State)]->Exit(Context);
	States[static_cast<uint8>(NewState)]->Enter(Context);
	State = NewState;
}

void AHook::BeginPlay()
{
	Super::BeginPlay();

	HookMesh->SetVisibility(false);
	Cable->SetVisibility(false);

	Cable->AttachToComponent(HookMesh,
		FAttachmentTransformRules::KeepRelativeTransform, EndPointSocket);

	if (GetLocalRole() >= ROLE_AutonomousProxy)
		AllocateState();
}

void AHook::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	OnRep_Owner();

	if (GetLocalRole() >= ROLE_AutonomousProxy)
		AllocateState();
}

void AHook::OnRep_Owner()
{
	if (const auto MyOwner = GetOwner<ACharacter>())
	{
		Cable->SetAttachEndToComponent(MyOwner->GetMesh(), HandSocket);
		SetRole(MyOwner->GetLocalRole());
	}
	else SetRole(ROLE_None);
}

void AHook::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const uint8 Idx = static_cast<uint8>(State);
	if (States.IsValidIndex(Idx))
		States[Idx]->Tick(Context, DeltaSeconds);
}

void AHook::GetLifetimeReplicatedProps(TArray
	<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHook, Context);
}

bool AHook::ReplicateSubobjects(UActorChannel* Channel,
	FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (Context)
		WroteSomething |= Channel->ReplicateSubobject(Context, *Bunch, *RepFlags);

	return WroteSomething;
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
	
	if (HasAuthority())
	{
		Context = NewObject<UHookContext>(this);
		OnRep_Context();
	}

	States.Emplace(new FIdleState);
	States.Emplace(new FThrowState);
	States.Emplace(new FSwingState);
	States.Emplace(new FMoveState);
}

void AHook::OnRep_Context()
{
	if (!bInit || bInitContext) return;
	bInitContext = true;

	FHookContextParam Param {
		this, CollisionProfile, HandSocket,
		Stat, HookTolerance, MoveTolerance
	};

	Context->Initialize(Param);
}
