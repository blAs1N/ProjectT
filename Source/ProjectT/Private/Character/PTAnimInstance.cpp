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

	const FVector Velocity = Owner->GetVelocity();
	const auto* Posture = Owner->GetPostureComp();
	const EPostureState CurState = Posture->GetPostureState();
	bIsSpinting = !Velocity.IsZero() && Posture->IsSprinting();

	const float MaxSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;
	Speed = Velocity.Size2D() / MaxSpeed;
	
	const FRotator Rotation = Owner->GetActorRotation();
	Direction = CalculateDirection(Velocity, Rotation);

	const FRotator AimRot = (Owner->GetBaseAimRotation() - Rotation).GetNormalized();;
	AimPitch = AimRot.Pitch; AimYaw = AimRot.Yaw;

	if (State != CurState)
	{
		const uint8 From = static_cast<uint8>(State);
		uint8 To = static_cast<uint8>(CurState);
		if (From < To) --To;

		Montage_Play((&PostureSwitchAnims.StandToCrouch)[From * 2 + To]);
	}

	State = CurState;
}
