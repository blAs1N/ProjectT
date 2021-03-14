// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/SoftObjectPtr.h"
#include "Component/CompositeModelComponent.h"
#include "Component/HookComponent.h"
#include "Component/WeaponComponent.h"
#include "Data/CharacterData.h"
#include "MISC/DataTableLoader.h"

APTCharacter::APTCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCompositeModelComponent>(MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

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

	// Get knockback power (Referenced by smash brothers)
	float Nnockback = 7.0f * (Damage + 2.0f) * (Damage + BackPercent);
	Nnockback /= (Weight + 100.0f);
	Nnockback *= 2.0f;
	Nnockback += BackConstant;

	LaunchCharacter(Dir * Nnockback, true, true);
	BackPercent += Damage;
	return Damage;
}

void APTCharacter::OnInitialize(int32 Key)
{
	GetData<FCharacterData>(CharacterDataTable, Key,
		[this](auto Data) { OnGetData(Data); }, bLoadAsync);

	IInitializable::Execute_Initialize(HookComp, Key);
	IInitializable::Execute_Initialize(WeaponComp, Key);
}

void APTCharacter::OnGetData(const FCharacterData& Data)
{
	Cast<UCompositeModelComponent>(GetMesh())->SetParam(Data.ModelParam);
	GetMesh()->SetRelativeRotation(FRotator{ 0.0f, Data.MeshYaw, 0.0f });
	GetMesh()->SetRelativeLocation(FVector{ 0.0f, 0.0f, Data.MeshZ });
	
	GetCapsuleComponent()->SetCapsuleSize(Data.CapsuleRadius, Data.CapsuleHalfHeight);
	Weight = Data.Weight;
}
