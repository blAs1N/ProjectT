// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTCharacter.h"
#include "Component/CompositeModelComponent.h"
#include "Data/CharacterData.h"
#include "Equipment/Weapon.h"
#include "Library/AsyncLoad.h"

APTCharacter::APTCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCompositeModelComponent>(MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
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

	const auto PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == TEXT("DataTable") || PropertyName == TEXT("RowName"))
		Initialize();
}

#endif

void APTCharacter::Initialize()
{
	static const FCharacterData DefaultData{};

	const auto* Data = CharacterKey.GetRow<FCharacterData>(TEXT(""));
	if (!Data) Data = &DefaultData;

	GetMesh()->SetRelativeRotation(FRotator{ 0.0f, Data->MeshYaw, 0.0f });
	GetMesh()->SetAnimClass(Data->AnimClass);
	AsyncLoad(Data->Mesh, [this, Data]
		{ GetMesh()->SetSkeletalMesh(Data->Mesh.Get()); });

	Weapon->Initialize(CharacterKey.RowName);
	Weight = Data->Weight;
}
