// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Data/PostureState.h"
#include "PTAnimInstance.generated.h"

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

UCLASS()
class PROJECTT_API UPTAnimInstance final : public UAnimInstance
{
	GENERATED_BODY()

private:
	void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FPostureSwitchAnims PostureSwitchAnims;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float AimPitch;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float AimYaw;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EPostureState State;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsSpinting : 1;
};
