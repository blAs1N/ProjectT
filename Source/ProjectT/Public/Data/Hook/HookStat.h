#pragma once

#include "HookStat.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FHookStat
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.1))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BoostPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxBoostPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMoveDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PenetrationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EndMoveLaunchPower;
};
