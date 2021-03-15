#pragma once

#include "Engine/DataTable.h"
#include "Data/Weapon/WeaponStat.h"
#include "WeaponData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HandSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MuzzleSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponStat Stat;
};
