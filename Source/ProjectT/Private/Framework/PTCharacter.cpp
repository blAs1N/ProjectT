// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Framework/PTCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Data/CharacterData.h"
#include "Framework/WeaponComponent.h"
#include "Library/PTStatics.h"

APTCharacter::APTCharacter()
	: Super(), Level(1u)
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	WeaponComp->SetupAttachment(GetMesh());
}

void APTCharacter::Heal(float Value)
{
	check(HasAuthority());
	if (Value == 0.0f) return;

	Health = FMath::Clamp(Health + Value, 0.0f, Stat.Health);
}

void APTCharacter::AddExp(float Value)
{
	check(HasAuthority());

	CurExp += Value;
	while (CurExp >= CurMaxExp)
	{
		++Level;
		CurMaxExp += ExpIncValue;
		Stat.Health += Stat.HealthInc;
	}

	Health = Stat.Health;
}

#if WITH_EDITOR

void APTCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!PropertyChangedEvent.Property) return;
	
	const auto PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == TEXT("DataTable") || PropertyName == TEXT("RowName"))
		Initialize();
}

#endif

void APTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Initialize();
}

void APTCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = Stat.Health;
	CurMaxExp = MaxExp;
}

float APTCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (Damage <= 0.0f) return 0.0f;

	Health = FMath::Max(Health - Damage, 0.0f);
	if (Health == 0.0f) Death();

	return Damage;
}

void APTCharacter::Initialize()
{
	static const FCharacterData DefaultData{};

	const auto* Data = CharacterKey.GetRow<FCharacterData>(TEXT(""));
	if (!Data) Data = &DefaultData;

	GetCapsuleComponent()->SetCapsuleSize(Data->CapsuleRadius, Data->CapsuleHalfHeight);
	GetMesh()->SetRelativeLocationAndRotation(Data->MeshLocation, Data->MeshRotation);
	GetMesh()->SetAnimClass(Data->AnimClass);
	
	FPTStatics::AsyncLoad(Data->Mesh, [this, Data]
		{ GetMesh()->SetSkeletalMesh(Data->Mesh.Get()); });

	Stat = Data->StatData;
	WeaponComp->Initialize(Data->WeaponData);
}

void APTCharacter::Death()
{

}