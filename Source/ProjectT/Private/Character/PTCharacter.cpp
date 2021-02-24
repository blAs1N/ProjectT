// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/SoftObjectPtr.h"
#include "Component/CompositeModelComponent.h"
#include "Component/WeaponComponent.h"
#include "Data/CharacterData.h"
#include "Library/AsyncLoad.h"

APTCharacter::APTCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCompositeModelComponent>(MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
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

void APTCharacter::PostInitProperties()
{
	Super::PostInitProperties();
	Initialize();
}

#if WITH_EDITOR

void APTCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!PropertyChangedEvent.Property) return;

	static const FName DataTableName = GET_MEMBER_NAME_CHECKED(APTCharacter, CharacterDataTable);
	static const FName KeyName = GET_MEMBER_NAME_CHECKED(APTCharacter, CharacterKey);
	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	
	if (PropertyName == DataTableName || PropertyName == KeyName)
		Initialize();
}
#endif

void APTCharacter::Initialize()
{
	if (!bLoadingAsset && CharacterKey != AppliedKey)
	{
		bLoadingAsset = true;
		AppliedKey = CharacterKey;

		AsyncLoad(CharacterDataTable, [this](auto DataTable) { OnLoadDataTable(DataTable); });
	}
}

void APTCharacter::OnLoadDataTable(const TSoftObjectPtr<class UDataTable>& DataTable)
{
	static const FCharacterData DefaultData{};
	const auto* Data = &DefaultData;

	if (DataTable.IsValid())
	{
		const auto* TempData = DataTable.Get()->FindRow
			<FCharacterData>(FName{ *FString::FromInt(AppliedKey) }, TEXT(""));

		if (TempData) Data = TempData;
	}

	Cast<UCompositeModelComponent>(GetMesh())->SetParam(Data->ModelParam);
	GetMesh()->SetRelativeRotation(FRotator{ 0.0f, Data->MeshYaw, 0.0f });
	GetMesh()->SetRelativeLocation(FVector{ 0.0f, 0.0f, Data->MeshZ });
	
	GetCapsuleComponent()->SetCapsuleSize(Data->CapsuleRadius, Data->CapsuleHalfHeight);

	Weight = Data->Weight;
	WeaponComp->Initialize(AppliedKey);

	bLoadingAsset = false;
}
