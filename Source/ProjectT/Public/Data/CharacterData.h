#pragma once

#include "Engine/DataTable.h"
#include "Data/PostureData.h"
#include "Data/StatData.h"
#include "Data/WeaponData.h"
#include "CharacterData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TAssetPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeshYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FStatData StatData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPostureData PostureData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponData WeaponData;
};