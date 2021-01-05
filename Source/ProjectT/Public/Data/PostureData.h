#pragma once

#include "UObject/ObjectMacros.h"
#include "PostureData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FPostureStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float SpeedRatio = 1.0f;

	UPROPERTY(EditAnywhere)
	float HalfHeight;

	UPROPERTY(EditAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere)
	float MeshOffset;
};

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FPostureData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FPostureStat StandData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FPostureStat CrouchData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FPostureStat ProneData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float SprintSpeedRatio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float DefaultWalkSpeed;
};