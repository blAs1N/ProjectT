// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/SwingState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/Hook/HookStat.h"
#include "Equipment/Hook.h"
#include "Equipment/HookContext.h"

void FSwingState::Enter(UHookContext* Context)
{
	const FVector HookLoc = Context->GetHookLocation();
	const FRotator HookRot = Context->GetHookRotation();

	const float Offset = Context->GetStat().PenetrationOffset;
	const auto Target = Context->GetHookTarget();
	
	const auto Hook = Context->GetHook();
	Hook->SetActorLocationAndRotation(HookLoc, HookRot);
	Hook->AddActorLocalRotation(FRotator{ 180.0f, 0.0f, 0.0f });
	Hook->AddActorLocalOffset(FVector{ Offset, 0.0f, 0.0f });
	Hook->AttachToComponent(Target, FAttachmentTransformRules::KeepWorldTransform);

	const float Length = FVector::Distance(Context->GetHandLocation(), HookLoc);
	Context->SetLength(Length);
}

void FSwingState::Tick(UHookContext* Context, float DeltaSeconds)
{
	const auto Target = Context->GetTarget();

	const FVector HookLoc = Context->GetHookLocation();
	const float Length = Context->GetLength();

	float ForceCoef = FVector::Dist2D(Context->GetHandLocation(), HookLoc);
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

		Velocity = ForceCoef * Context->GetStat()
			.BoostPower * FMath::Clamp(Velocity, 0.0f, 1.0f);

		Force = (TargetVelocity * Velocity)
			.GetClampedToMaxSize(Context->GetStat().MaxBoostPower);

		Target->GetCharacterMovement()->AddForce(Force);
	}

	Velocity = FMath::Max(FVector::Distance(TargetLoc, HookLoc) - Length, 0.0f) * 400.0f;

	Force = FRotationMatrix::MakeFromX(HookLoc - TargetLoc).Rotator().Vector();
	Target->GetCharacterMovement()->AddImpulse(Force * Velocity);
}
