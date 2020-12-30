#pragma once

#include "Engine/DataTable.h"
#include "Data/StatData.h"
#include "Data/WeaponData.h"
#include "CharacterData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStatData StatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponData WeaponData;
};