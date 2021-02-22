// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTT_API UWeaponComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

	void Initialize(uint32 InKey);

private:
	void OnLoadDataTable(const TSoftObjectPtr<class UDataTable>& DataTable);

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UDataTable> WeaponDataTable;

	const struct FWeaponStat* Stat;

	uint32 Key;
	uint32 Clip;
	uint8 bLoadingAsset : 1;
};
