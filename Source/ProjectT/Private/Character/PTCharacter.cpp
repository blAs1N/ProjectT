// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/CompositeModelComponent.h"
#include "Data/CharacterData.h"
#include "Equipment/Weapon.h"
#include "Library/AsyncLoad.h"

APTCharacter::APTCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCompositeModelComponent>(MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
}

float APTCharacter::TakeDamage(float Damage, const FDamageEvent&
	DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (Damage <= 0.0f) return 0.0f;

	FVector Dir;
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const auto& PointDamageEvent = reinterpret_cast<const FPointDamageEvent&>(DamageEvent);
		Dir = PointDamageEvent.ShotDirection;
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const auto& RadialDamageEvent = reinterpret_cast<const FRadialDamageEvent&>(DamageEvent);
		Dir = GetActorLocation() - RadialDamageEvent.Origin;
		Dir.Normalize();
	}

	float Nnockback = 7.0f * (Damage + 2.0f) * (Damage + BackPercent);
	Nnockback /= (Weight + 100.0f);
	Nnockback *= 2.0f;
	Nnockback += BackConstant;

	GetCharacterMovement()->AddImpulse(Dir * Nnockback, true);
	BackPercent += Damage;
	return Damage;
}

void APTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters Params;
		Params.Owner = Params.Instigator = this;
		Weapon = World->SpawnActor<AWeapon>(WeaponClass, Params);
	}

	Initialize();
}

#if WITH_EDITOR

void APTCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!PropertyChangedEvent.Property) return;

	const static auto DataTableName = GET_MEMBER_NAME_CHECKED(APTCharacter, CharacterDataTable);
	const static auto CharKeyName = GET_MEMBER_NAME_CHECKED(APTCharacter, CharacterKey);

	const auto PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == DataTableName || PropertyName == CharKeyName)
		Initialize();
}

#endif

void APTCharacter::Initialize()
{
	static const FCharacterData DefaultData{};
	if (!CharacterDataTable) return;

	if (const auto* Data = CharacterDataTable->
		FindRow<FCharacterData>(FName{ *FString::FromInt(CharacterKey) }, TEXT("")))
	{
		Cast<UCompositeModelComponent>(GetMesh())->SetParam(Data->ModelParam);
		
		GetMesh()->SetRelativeRotation(FRotator{ 0.0f, Data->MeshYaw, 0.0f });
		GetMesh()->SetRelativeLocation(FVector{ 0.0f, 0.0f, Data->MeshZ });
		
		Weapon->Initialize(CharacterKey);
		Weight = Data->Weight;
	}
}
