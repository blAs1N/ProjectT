// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/SwingState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Equipment/Hook.h"

void FSwingState::Enter()
{
	const auto Hook = GetOwner<AHook>();

	const FVector HookLoc = Hook->GetHookLocation();
	const FRotator HookRot = Hook->GetHookRotation();

	const float Offset = Hook->GetStat().PenetrationOffset;
	const auto Target = Hook->GetHookedTarget();
	
	Hook->SetActorLocationAndRotation(HookLoc, HookRot);
	Hook->AddActorLocalRotation(FRotator{ 180.0f, 0.0f, 0.0f });
	Hook->AddActorLocalOffset(FVector{ Offset, 0.0f, 0.0f });
	Hook->AttachToComponent(Target, FAttachmentTransformRules::KeepWorldTransform);

	const float Length = FVector::Distance(Hook->GetHandLocation(), HookLoc);
	Hook->SetLength(Length);
}

void FSwingState::Tick(float DeltaSeconds)
{
	const auto Hook = GetOwner<AHook>();
	const auto Target = Hook->GetOwner<ACharacter>();

	const FVector HookLoc = Hook->GetHookLocation();
	const float Length = Hook->GetLength();

	float ForceCoef = FVector::Dist2D(Hook->GetHandLocation(), HookLoc);
	ForceCoef /= Length;
	ForceCoef = 1.0f - ForceCoef;
	ForceCoef = FMath::Clamp(ForceCoef, 0.0f, 1.0f);

	const FVector TargetLoc = Target->GetActorLocation();
	const FVector TargetVelocity = Target->GetVelocity();
	
	FVector Force; float Velocity;
	if (TargetLoc.Z < HookLoc.Z)
	{
		Force = TargetLoc - HookLoc;
		Velocity = TargetVelocity | Force;
		Force.Normalize();

		Target->GetCharacterMovement()->
			AddForce(Force * Velocity * -2.0f * ForceCoef);

		Velocity = TargetVelocity.GetSafeNormal()
			| Target->GetControlRotation().Vector();

		Velocity = ForceCoef * Hook->GetStat()
			.BoostPower * FMath::Clamp(Velocity, 0.0f, 1.0f);

		Force = (TargetVelocity * Velocity)
			.GetClampedToMaxSize(Hook->GetStat().MaxBoostPower);

		Target->GetCharacterMovement()->AddForce(Force);
	}

	Velocity = FMath::Max(FVector::Distance(TargetLoc, HookLoc) - Length, 0.0f) * 400.0f;

	Force = FRotationMatrix::MakeFromX(HookLoc - TargetLoc).Rotator().Vector();
	Target->GetCharacterMovement()->AddImpulse(Force * Velocity);
}
