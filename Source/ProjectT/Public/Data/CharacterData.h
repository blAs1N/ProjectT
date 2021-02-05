#pragma once

#include "Engine/DataTable.h"
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
	float Weight;
};
