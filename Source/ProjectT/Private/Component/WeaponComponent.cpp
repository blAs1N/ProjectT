// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/WeaponComponent.h"
#include "Engine/DataTable.h"
#include "Character/PTCharacter.h"
#include "Data/WeaponData.h"
#include "Library/AsyncLoad.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::Initialize(uint32 InKey)
{
	if (bLoadingAsset) return;

	bLoadingAsset = true;
	Key = InKey;

	AsyncLoad(WeaponDataTable, [this](auto DataTable) { OnLoadDataTable(DataTable); });
}

void UWeaponComponent::OnLoadDataTable(const TSoftObjectPtr<UDataTable>& DataTable)
{
	static const FWeaponData DefaultData{};
	const auto* Data = &DefaultData;

	if (DataTable.IsValid())
	{
		const auto* TempData = DataTable.Get()->FindRow
			<FWeaponData>(FName{ *FString::FromInt(Key) }, TEXT(""));

		if (TempData) Data = TempData;
	}

	const auto* Owner = Cast<APTCharacter>(GetOwner());
	check(Owner);

	auto* const WeaponMesh = Owner->GetWeaponMeshComponent();
	AsyncLoad(Data->Mesh, [WeaponMesh](const auto& Ptr)
		{
			WeaponMesh->SetSkeletalMesh(Ptr.Get());
		});

	WeaponMesh->SetAnimClass(Data->AnimClass);
	WeaponMesh->SetRelativeTransform(Data->Transform);
	WeaponMesh->AttachToComponent(Owner->GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform, Data->Socket);

	Stat = &Data->Stat;
	bLoadingAsset = false;
}
