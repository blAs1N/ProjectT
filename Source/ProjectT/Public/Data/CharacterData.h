#pragma once

#include "Engine/DataTable.h"
#include "Data/CompositeModelParam.h"
#include "CharacterData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeshZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeshYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
};
