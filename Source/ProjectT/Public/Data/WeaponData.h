#pragma once

#include "Data/WeaponStatData.h"
#include "WeaponData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Socket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponStatData Stat;
};
