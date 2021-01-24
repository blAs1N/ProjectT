// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTCameraManager.h"
#include "Character/PostureComponent.h"
#include "Character/PTCharacter.h"
#include "Character/PTPlayerController.h"
#include "Character/WeaponComponent.h"

void APTCameraManager::BeginPlay()
{
	Super::BeginPlay();
	
	CameraInfo.FOV = TargetInfo.FOV = UnaimFOV;

	if (AimChangeCurve)
	{
		FOnTimelineFloat OnAimChange;
		OnAimChange.BindDynamic(this, &APTCameraManager::ChangeCameraInfo);
		AimChangeTimeline.AddInterpFloat(AimChangeCurve, OnAimChange);
	}
}

void APTCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	auto* Target = Cast<APTCharacter>(OutVT.Target);
	if (!Target)
	{
		Super::UpdateViewTargetInternal(OutVT, DeltaTime);
		return;
	}

	ApplyViewPitch(Target->GetPostureComp()->GetPostureState());

	const bool bIsAim = Target->GetWeaponComp()->IsAiming();
	TargetInfo.Rot = Target->GetControlRotation();
	TargetInfo.Loc = bIsAim ? GetAimCamLoc(Target) : GetUnaimCamLoc(Target);

	if (bIsAim != bIsAiming)
	{
		TargetInfo.FOV = (bIsAim ? AimFOV : UnaimFOV);
		ChangeInfo = CameraInfo;

		AimChangeTimeline.PlayFromStart();
	}

	bIsAiming = bIsAim;
	
	if (AimChangeCurve && AimChangeTimeline.IsPlaying())
		AimChangeTimeline.TickTimeline(DeltaTime);
	else
	{
		CameraInfo.Loc = TargetInfo.Loc;
		CameraInfo.Rot = TargetInfo.Rot;
	}

	OutVT.POV.Location = CameraInfo.Loc;
	OutVT.POV.Rotation = CameraInfo.Rot;
	OutVT.POV.FOV = CameraInfo.FOV;
}

void APTCameraManager::ApplyViewPitch(EPostureState NewState)
{
	FViewPitch ViewPitch = ViewPitchs[static_cast<uint8>(NewState)];
	ViewPitchMin = ViewPitch.Min;
	ViewPitchMax = ViewPitch.Max;
}

void APTCameraManager::ChangeCameraInfo(float Value)
{
	CameraInfo.Loc = FMath::Lerp(ChangeInfo.Loc, TargetInfo.Loc, Value);
	CameraInfo.Rot = FMath::RInterpTo(ChangeInfo.Rot, TargetInfo.Rot, 1.0f, Value);
	CameraInfo.FOV = FMath::Lerp(ChangeInfo.FOV, TargetInfo.FOV, Value);
}

FVector APTCameraManager::GetUnaimCamLoc(APTCharacter* Target)
{
	const FVector ViewLoc = Target->GetMesh()->GetSocketLocation(UnaimViewPointName);
	const FVector Offset = TargetInfo.Rot.RotateVector(TargetOffset);
	const FVector RetLoc = ViewLoc + Offset;

	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Target);

	const bool bCollide = GetWorld()->SweepSingleByChannel(Result, ViewLoc, RetLoc,
		FQuat::Identity, ECollisionChannel::ECC_Camera, FCollisionShape::MakeSphere(ProbeSize), Params);

	return bCollide ? Result.Location : RetLoc;
}

FVector APTCameraManager::GetAimCamLoc(APTCharacter* Target)
{
	return Target->GetWeaponComp()->GetSocketLocation(AimViewPointName);
}
