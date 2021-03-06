#pragma once

#include "CompositeModelParam.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FCompositeModelParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class USkeleton> Skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<class USkeletalMesh>> Pieces;
};
