// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Framework/PTCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

APTCharacter::APTCharacter()
	: Level(1u)
{
	PrimaryActorTick.bCanEverTick = true;
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
	while (CurExp >= MaxExp)
	{
		++Level;
		MaxExp += ExpIncValue;
		Stat.Health += Stat.HealthIncValue;
	}
}

void APTCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = Stat.Health;
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

void APTCharacter::Death()
{

}