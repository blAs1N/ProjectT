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
struct PROJECTT_API FPostureSwitchAnims
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* StandToCrouch;

	UPROPERTY(EditAnywhere)
	UAnimMontage* StandToProne;

	UPROPERTY(EditAnywhere)
	UAnimMontage* CrouchToStand;

	UPROPERTY(EditAnywhere)
	UAnimMontage* CrouchToProne;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ProneToStand;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ProneToCrouch;
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
	FPostureSwitchAnims PostureSwitchAnims;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float SprintSpeedRatio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float DefaultWalkSpeed;
};