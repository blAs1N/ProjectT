// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Data/WeaponData.h"
#include "Library/AsyncLoad.h"

void AWeapon::Initialize(int32 Key)
{
	const auto OwnerChar = Cast<ACharacter>(GetOwner());
	check(OwnerChar);
	
	if (!WeaponDataTable) return;
	
	if (const auto* Data = WeaponDataTable->FindRow<FWeaponData>
		(FName{ *FString::FromInt(Key) }, TEXT("")))
	{
		Mesh->SetAnimClass(Data->AnimClass);
		AsyncLoad(Data->Mesh, [Mesh = Mesh, Data]
			{
				Mesh->SetSkeletalMesh(Data->Mesh.Get());
			});

		SetActorRelativeTransform(Data->Transform);
		AttachToComponent(OwnerChar->GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform, Data->Socket);

		Stat = &Data->Stat;
	}
}
