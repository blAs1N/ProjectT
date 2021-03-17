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
	float MoveScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMoveDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EndMoveLaunchPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PenetrationOffset;
};
