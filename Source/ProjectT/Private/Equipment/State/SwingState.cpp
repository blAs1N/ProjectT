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
	Hook->AttachToActor(Target, FAttachmentTransformRules::KeepWorldTransform);

	Length = FVector::Distance(Context->GetHandLocation(), HookLoc);
	Context->SetLength(Length);

	DefaultAirCtrl = Context->GetTarget()->GetCharacterMovement()->AirControl;
	Context->SetAirCtrl(1.0f);
}

void FSwingState::Tick(UHookContext* Context, float DeltaSeconds)
{
	const auto Target = Context->GetTarget();

	const FVector TargetVel = Target->GetVelocity();
	const FVector TargetLoc = Target->GetActorLocation();
	const FVector HookLoc = Context->GetHookLocation();
	
	const float Coef = FMath::Clamp(1.0f - (FVector::
		Dist2D(TargetLoc, HookLoc) / Length), 0.0f, 1.0f);

	if (TargetLoc.Z < HookLoc.Z)
	{
		FVector Force = TargetLoc - HookLoc;
		const float Dot = TargetVel | Force;

		Force.Normalize();
		Target->GetCharacterMovement()->AddForce(Force * Dot * -2.0f * Coef);

		const float Boost = Context->GetStat().BoostPower;
		const float MaxBoost = Context->GetStat().MaxBoostPower;

		Force = (TargetVel * Boost * Coef).GetClampedToMaxSize(MaxBoost);
		Target->GetCharacterMovement()->AddForce(Force);
	}
	
	const float Impulse = FMath::Max(FVector::Dist(
		TargetLoc, HookLoc) - Length, 0.0f) * 400.0f;

	const FVector Dir = FRotationMatrix::
		MakeFromX(HookLoc - TargetLoc).Rotator().Vector();

	Target->GetCharacterMovement()->AddImpulse(Dir * Impulse);
}

void FSwingState::Exit(UHookContext* Context)
{
	Context->SetAirCtrl(DefaultAirCtrl);
}
