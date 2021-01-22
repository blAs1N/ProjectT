// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTCameraManager.h"
#include "Character/PostureComponent.h"
#include "Character/PTCharacter.h"
#include "Character/PTPlayerController.h"
#include "Character/WeaponComponent.h"

void APTCameraManager::InitializeFor(APlayerController* PC)
{
	Super::InitializeFor(PC);

	auto* PTPC = Cast<APTPlayerController>(PC);
	
	if (PTPC->GetPawn())
		OnPossessed(PTPC->GetPawn());
	else
		PTPC->OnPossessed.AddDynamic(this, &APTCameraManager::OnPossessed);

	if (AimChangeCurve)
	{
		FOnTimelineFloat OnAimChange;
		OnAimChange.BindDynamic(this, &APTCameraManager::ChangeCameraInfo);
		AimChangeTimeline.AddInterpFloat(AimChangeCurve, OnAimChange);
	}
}

void APTCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	if (!PTOwner)
	{
		Super::UpdateViewTargetInternal(OutVT, DeltaTime);
		return;
	}

	TargetRot = PTOwner->GetControlRotation();
	TargetLoc = bIsAiming ? GetAimCamLoc() : GetUnaimCamLoc();
	
	if (AimChangeCurve && AimChangeTimeline.IsPlaying())
		AimChangeTimeline.TickTimeline(DeltaTime);
	else
	{
		CameraLoc = TargetLoc;
		CameraRot = TargetRot;
		CameraFOV = TargetFOV;
	}

	OutVT.POV.Location = CameraLoc;
	OutVT.POV.Rotation = CameraRot;
	OutVT.POV.FOV = CameraFOV;
}

void APTCameraManager::OnPossessed(APawn* InPawn)
{
	PTOwner = Cast<APTCharacter>(InPawn);

	OnSwitchAim(PTOwner->GetWeaponComp()->IsAiming());
	OnSwitchPosture(PTOwner->GetPostureComp()->GetPostureState());

	PTOwner->GetWeaponComp()->OnSwitchAim.AddDynamic(this, &APTCameraManager::OnSwitchAim);
	PTOwner->GetPostureComp()->OnSwitchPosture.AddDynamic(this, &APTCameraManager::OnSwitchPosture);
}

void APTCameraManager::OnSwitchPosture(EPostureState NewState)
{
	FViewPitch ViewPitch = ViewPitchs[static_cast<uint8>(NewState)];
	ViewPitchMin = ViewPitch.Min;
	ViewPitchMax = ViewPitch.Max;
}

void APTCameraManager::OnSwitchAim(bool bIsAim)
{
	bIsAiming = bIsAim;
	TargetFOV = bIsAiming ? AimFOV : UnaimFOV;
	AimChangeTimeline.PlayFromStart();
}

FVector APTCameraManager::GetUnaimCamLoc()
{
	const FVector ViewLoc = PTOwner->GetMesh()->GetSocketLocation(UnaimViewPointName);
	const FVector Offset = TargetRot.RotateVector(TargetOffset);
	const FVector RetLoc = ViewLoc + Offset;
	
	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PTOwner);

	const bool bCollide = GetWorld()->SweepSingleByChannel(Result, ViewLoc, RetLoc,
		FQuat::Identity, ECollisionChannel::ECC_Camera, FCollisionShape::MakeSphere(ProbeSize), Params);

	return bCollide ? Result.Location : RetLoc;
}

FVector APTCameraManager::GetAimCamLoc()
{
	return PTOwner->GetWeaponComp()->GetSocketLocation(AimViewPointName);
}

void APTCameraManager::ChangeCameraInfo(float Value)
{
	CameraLoc = FMath::Lerp(CameraLoc, TargetLoc, Value);
	CameraRot = FMath::RInterpTo(CameraRot, TargetRot, 1.0f, Value);
	CameraFOV = FMath::Lerp(CameraFOV, TargetFOV, Value);
}
