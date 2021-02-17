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

void UWeaponComponent::Initialize(uint32 Key)
{
	const auto Owner = Cast<APTCharacter>(GetOwner());
	check(Owner);

	if (!WeaponDataTable) return;

	if (const auto* Data = WeaponDataTable->FindRow<FWeaponData>
		(FName{ *FString::FromInt(Key) }, TEXT("")))
	{
		auto* const WeaponMesh = Owner->GetWeaponMeshComponent();
		WeaponMesh->SetAnimClass(Data->AnimClass);
		AsyncLoad(Data->Mesh, [WeaponMesh](const auto& Ptr)
			{
				WeaponMesh->SetSkeletalMesh(Ptr.Get());
			});

		WeaponMesh->SetRelativeTransform(Data->Transform);
		WeaponMesh->AttachToComponent(Owner->GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform, Data->Socket);

		Stat = &Data->Stat;
	}
}

//void UWeaponComponent::StartFire()
//{
//
//}
//
//void UWeaponComponent::StopFire()
//{
//
//}
//
//void UWeaponComponent::Reload()
//{
//
//}
