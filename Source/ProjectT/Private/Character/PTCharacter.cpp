// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UObject/SoftObjectPtr.h"
#include "Component/CompositeModelComponent.h"
#include "Component/HookComponent.h"
#include "Component/WeaponComponent.h"
#include "Data/CharacterData.h"
#include "Game/PTGameMode.h"
#include "MISC/DataTableLoader.h"

APTCharacter::APTCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bOnlyOwnerSee = true;
	GetMesh()->CastShadow = false;

	ModelComp = CreateDefaultSubobject<UCompositeModelComponent>(TEXT("TPS Model"));
	ModelComp->SetupAttachment(RootComponent);
	ModelComp->bCastHiddenShadow = true;
	ModelComp->bOwnerNoSee = true;

	HookComp = CreateDefaultSubobject<UHookComponent>(TEXT("Hook"));
	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));

	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMeshComp->SetupAttachment(GetMesh());
	WeaponMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponMeshComp->SetNotifyRigidBodyCollision(false);
}

float APTCharacter::TakeDamage(float Damage, const FDamageEvent&
	DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (Damage <= 0.0f) return 0.0f;

	FVector Dir;
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const auto& PointDamageEvent = (const FPointDamageEvent&)DamageEvent;
		Dir = PointDamageEvent.ShotDirection;
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const auto& RadialDamageEvent = (const FRadialDamageEvent&)DamageEvent;
		Dir = GetActorLocation() - RadialDamageEvent.Origin;
		Dir.Normalize();
	}

	BackPercent += Damage;

	if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		AddActorWorldOffset(FVector{ 0.0f, 0.0f, 50.0f });
		Dir.Z = FMath::Max(Dir.Z, 0.0f);
		Dir.Normalize();
	}
	
	GetCharacterMovement()->AddImpulse(Dir * DamageToKnockback(Damage));
	return Damage;
}

void APTCharacter::Death()
{
	Destroy();
}

void APTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
		bInvincible = true;

	if (IsLocallyControlled())
		OnTakeAnyDamage.AddDynamic(this, &APTCharacter::OnHit);
}

void APTCharacter::OnInitialize(int32 InKey)
{
	Key = InKey;
	OnRep_Key();

	IInitializable::Execute_Initialize(HookComp, Key);
	IInitializable::Execute_Initialize(WeaponComp, Key);
}

void APTCharacter::OnGetData(const FCharacterData& Data)
{
	LoadObject(Data.FPSMesh, [Mesh = GetMesh()](const auto& Ptr)
	{
		Mesh->SetSkeletalMesh(Ptr.Get());
	}, bLoadAsync);

	LoadObject(Data.FPSAnimClass, [Mesh = GetMesh()](const auto& Ptr)
	{
		Mesh->SetAnimClass(Ptr.Get());
	}, bLoadAsync);

	ModelComp->SetParam(Data.ModelParam);

	const FVector MeshLoc{ 0.0f, 0.0f, Data.MeshZ };
	const FQuat MeshRot{ FRotator{ 0.0f, Data.MeshYaw, 0.0f } };

	GetMesh()->SetRelativeLocation(MeshLoc);
	ModelComp->SetRelativeLocationAndRotation(MeshLoc, MeshRot);

	BaseTranslationOffset = MeshLoc;

	GetCapsuleComponent()->SetCapsuleSize(Data.CapsuleRadius, Data.CapsuleHalfHeight);
	GetCharacterMovement()->Mass = Data.Weight;
	DeathDelay = Data.DeathDelay;
}

void APTCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (const auto* GM = GetWorld()->GetAuthGameMode<APTGameMode>())
	{
		if (GM->IsInsideZone(GetActorLocation()))
		{
			if (bInvincible)
				bInvincible = false;
			else
				DeathRemainTime = 0.0f;
		}
		else if (!bInvincible)
		{
			DeathRemainTime += DeltaSeconds;
			if (DeathRemainTime >= DeathDelay)
				Death();
		}
	}
}

void APTCharacter::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APTCharacter, Key);
}

bool APTCharacter::ShouldTakeDamage(float Damage, const FDamageEvent&
	DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	return Super::ShouldTakeDamage(Damage, DamageEvent,
		EventInstigator, DamageCauser) && !bInvincible;
}

void APTCharacter::OnRep_Key()
{
	GetData<FCharacterData>(CharacterDataTable, Key,
		[this](const auto& Data) { OnGetData(Data); }, bLoadAsync);
}

void APTCharacter::OnHit(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HookComp->Unhook();
}
