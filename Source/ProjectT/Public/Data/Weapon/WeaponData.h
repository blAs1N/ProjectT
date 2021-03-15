#pragma once

#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "WeaponData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class USkill> Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UDataAsset> Data;
};

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSkillData> Skills;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HandSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MuzzleSocket;
};
