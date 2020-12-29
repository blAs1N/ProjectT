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

	Health = FMath::Clamp(Health + Value, 0.0f, StatData.Health);
}

void APTCharacter::AddExp(float Value)
{
	check(HasAuthority());

	CurExp += Value;
	while (CurExp >= MaxExp)
	{
		++Level;
		MaxExp += ExpIncValue;
		StatData.Health += StatData.HealthInc;
	}
}

#if WITH_EDITOR

void APTCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	static const FName& CharKeyName = GET_MEMBER_NAME_CHECKED(APTCharacter, CharacterKey);
	if (PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == CharKeyName)
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

	Health = StatData.Health;
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
	if (const auto* Data = CharacterKey.GetRow<FCharacterData>(TEXT("")))
	{
		FPTStatics::AsyncLoad(Data->Mesh, [this, Data]
		{
			GetCapsuleComponent()->SetCapsuleSize(Data->CapsuleRadius, Data->CapsuleHalfHeight);

			GetMesh()->SetAnimClass(Data->AnimClass);
			GetMesh()->SetSkeletalMesh(Data->Mesh.Get());

			const FVector& Location = GetMesh()->GetRelativeLocation();
			const FRotator& Rotation = GetMesh()->GetRelativeRotation();

			GetMesh()->SetRelativeLocationAndRotation(
				FVector{ Location.X, Location.Y, Data->MeshZ },
				FRotator{ Rotation.Pitch, Data->MeshYaw, Rotation.Roll }
			);
		});

		StatData = Data->StatData;
		WeaponComp->Initialize(Data->WeaponData);
	}
}

void APTCharacter::Death()
{

}