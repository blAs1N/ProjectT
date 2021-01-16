// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PostureComponent.h"
#include "Character/PTCharacter.h"
#include "Character/WeaponComponent.h"

void UPTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	const auto* Owner = Cast<APTCharacter>(TryGetPawnOwner());
	if (!Owner) return;
	
	bNeedIK = !Owner->IsDeath() && !Owner->GetWeaponComp()->IsReloading();
	if (bNeedIK)
	{
		const auto LeftHand = Owner->GetWeaponComp()->GetSocketTransform(WeaponIKName);
		FVector IKLoc;	FRotator IKRot;
		Owner->GetMesh()->TransformToBoneSpace(RightHandName,
			LeftHand.GetLocation(), LeftHand.GetRotation().Rotator(), IKLoc, IKRot);

		LeftHandIK.SetLocation(IKLoc);
		LeftHandIK.SetRotation(IKRot.Quaternion());

		float CurSpineYaw = 0.0f;
		FVector Velocity = Owner->GetVelocity();
		Velocity.Z = 0.0f;
	}

	float CurSpineYaw = 0.0f;
	FVector Velocity = Owner->GetVelocity();
	Velocity.Z = 0.0f;

	if (!Velocity.IsNearlyZero(JogCriterion))
	{
		FRotator DeltaRot = Velocity.Rotation() - Owner->GetActorRotation();
		DeltaRot.Normalize();
		
		CurSpineYaw = FMath::Sin(FMath::DegreesToRadians(DeltaRot.Yaw)) * RotationScale;

		Direction = FMath::Abs(DeltaRot.Yaw) <= 100.0f
			? EDirection::Forward : EDirection::Backward;

		if (Direction == EDirection::Backward)
			CurSpineYaw *= -1.0f;
	}
	else Direction = EDirection::Idle;

	SpineYaw = FMath::Lerp(SpineYaw, CurSpineYaw, DeltaSeconds * RotationSpeed);

	const FRotator AimRot = (Owner->GetBaseAimRotation() - Owner->GetActorRotation()).GetNormalized();;
	AimPitch = AimRot.Pitch; AimYaw = AimRot.Yaw;

	const auto* Posture = Owner->GetPostureComp();
	State = Posture->GetPostureState();

	bIsSpinting = Posture->IsSprinting();
	bIsFalling = Owner->GetCharacterMovement()->IsFalling();
	bIsAiming = Owner->GetWeaponComp()->IsAiming();
} 
