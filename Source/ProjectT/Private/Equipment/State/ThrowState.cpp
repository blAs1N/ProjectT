// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/ThrowState.h"
#include "Equipment/Hook.h"

void FThrowState::Enter()
{
	const auto Hook = GetOwner<AHook>();
	
	Hook->TraceHookTarget();
	StartLoc = Hook->GetHandLocation();

	const FRotator Rot = FRotationMatrix::
		MakeFromX(Hook->GetHookLocation() - StartLoc).Rotator();

	Hook->SetActorLocationAndRotation(StartLoc, Rot);
	Hook->SetVisibility(true);
}

void FThrowState::Tick(float DeltaSeconds)
{
	const auto Hook = GetOwner<AHook>();
	
	const FVector HookLoc = Hook->GetHookLocation();
	const float Dist = FVector::Distance(StartLoc, HookLoc);
	const FVector NewLoc = FMath::Lerp(Hook->GetActorLocation(),
		HookLoc, FMath::Min(DeltaSeconds * Hook->GetStat().Speed, 1.0f));

	Hook->SetActorLocation(NewLoc);

	const float TargetDist = FVector::DistSquared(NewLoc, HookLoc);
	const float HookDist = FVector::DistSquared(StartLoc, NewLoc);

	const float HookTolerance = Hook->GetHookTolerance();
	const float Distance = Hook->GetStat().Distance;

	if (TargetDist < HookTolerance * HookTolerance)
		Hook->SetState(Hook->GetHookedTarget() ? EHookState::Swing : EHookState::Idle);
	else if (HookDist > Distance * Distance)
		Hook->SetState(EHookState::Idle);
}
