// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/WeaponComponent.h"
#include "Engine/DataTable.h"
#include "Character/PTCharacter.h"
#include "Data/WeaponData.h"
#include "MISC/AsyncLoad.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::Initialize(uint32 InKey)
{
	if (bLoadingAsset && WeaponDataTable.IsNull()) return;

	bLoadingAsset = true;
	Key = InKey;

	const bool bLoadAsync = ILoadable::Execute_IsLoadAsync(GetOwner());
	if (bLoadAsync)
	{
		AsyncLoad(WeaponDataTable, [this](auto DataTable) { OnLoadDataTable(DataTable); });
	}
	else
	{
		WeaponDataTable.LoadSynchronous();
		OnLoadDataTable(WeaponDataTable);
	}
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

	const bool bLoadAsync = ILoadable::Execute_IsLoadAsync(Owner);
	auto* const WeaponMesh = Owner->GetWeaponMeshComponent();

	if (bLoadAsync)
	{
		AsyncLoad(Data->Mesh, [WeaponMesh](const auto& Ptr)
			{
				WeaponMesh->SetSkeletalMesh(Ptr.Get());
			});
	}
	else
	{
		WeaponMesh->SetSkeletalMesh(Data->Mesh.LoadSynchronous());
	}

	WeaponMesh->SetAnimClass(Data->AnimClass);
	WeaponMesh->SetRelativeTransform(Data->Transform);
	WeaponMesh->AttachToComponent(Owner->GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform, Data->Socket);

	Stat = &Data->Stat;
	bLoadingAsset = false;
}
