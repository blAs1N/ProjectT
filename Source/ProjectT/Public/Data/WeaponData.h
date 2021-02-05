#pragma once

#include "Data/WeaponStat.h"
#include "WeaponData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TAssetPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Socket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponStat Stat;
};
