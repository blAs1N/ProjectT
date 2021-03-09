// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/WeaponComponent.h"
#include "Engine/DataTable.h"
#include "Character/PTCharacter.h"
#include "Data/WeaponData.h"
#include "MISC/DataTableLoader.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::OnInitialize(int32 InKey)
{
	Key = InKey;
}
