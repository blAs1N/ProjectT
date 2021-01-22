// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/TimelineComponent.h"
#include "Data/PostureState.h"
#include "PTCameraManager.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FViewPitch
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Min;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Max;
};

UCLASS()
class PROJECTT_API APTCameraManager final : public APlayerCameraManager
{
	GENERATED_BODY()
	
private:
	void InitializeFor(APlayerController* PC) override;
	void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UFUNCTION()
	void OnPossessed(APawn* InPawn);

	UFUNCTION()
	void OnSwitchPosture(EPostureState NewState);

	UFUNCTION()
	void OnSwitchAim(bool bIsAim);

	UFUNCTION()
	void ChangeCameraInfo(float Value);

	FVector GetUnaimCamLoc();
	FVector GetAimCamLoc();

private:
	UPROPERTY(Transient)
	class APTCharacter* PTOwner;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = true))
	FViewPitch ViewPitchs[3];

	UPROPERTY(EditAnywhere, Category = ViewPointName, meta = (AllowPrivateAccess = true))
	FName UnaimViewPointName;

	UPROPERTY(EditAnywhere, Category = ViewPointName, meta = (AllowPrivateAccess = true))
	FName AimViewPointName;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = true))
	FVector TargetOffset;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = true))
	UCurveFloat* AimChangeCurve;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = true, ClampMin = 0.1))
	float ProbeSize;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = true, ClampMin = 0.0, ClampMax = 180.0))
	float UnaimFOV;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = true, ClampMin = 0.0, ClampMax = 180.0))
	float AimFOV;

	FTimeline AimChangeTimeline;

	FVector CameraLoc;
	FVector TargetLoc;

	FRotator CameraRot;
	FRotator TargetRot;

	float CameraFOV;
	float TargetFOV;

	uint8 bIsAiming : 1;
};
