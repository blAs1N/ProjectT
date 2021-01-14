// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Data/PostureState.h"
#include "PTAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Idle, Forward, Backward
};

UCLASS()
class PROJECTT_API UPTAnimInstance final : public UAnimInstance
{
	GENERATED_BODY()

private:
	void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere)
	float JogCriterion;

	UPROPERTY(EditAnywhere)
	float RotationScale;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

private:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float SpineYaw;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float AimPitch;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float AimYaw;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EDirection Direction;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EPostureState State;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsSpinting : 1;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsFalling : 1;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsAiming : 1;
};
