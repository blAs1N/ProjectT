// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/ThrowState.h"
#include "Data/Hook/HookStat.h"
#include "Equipment/Hook.h"
#include "Equipment/HookContext.h"

void FThrowState::Enter(UHookContext* Context)
{
	Context->TraceHookTarget();
	StartLoc = Context->GetHandLocation();

	const FRotator Rot = FRotationMatrix::
		MakeFromX(Context->GetHookLocation() - StartLoc).Rotator();

	Context->GetHook()->SetActorLocationAndRotation(StartLoc, Rot);
	Context->SetVisibility(true);
}

void FThrowState::Tick(UHookContext* Context, float DeltaSeconds)
{
	const auto Hook = Context->GetHook();
	
	const FVector HookLoc = Context->GetHookLocation();
	const float Dist = FVector::Distance(StartLoc, HookLoc);
	const FVector NewLoc = FMath::Lerp(Hook->GetActorLocation(),
		HookLoc, FMath::Min(DeltaSeconds * Context->GetStat().Speed, 1.0f));

	Hook->SetActorLocation(NewLoc);

	const float TargetDist = FVector::DistSquared(NewLoc, HookLoc);
	const float HookDist = FVector::DistSquared(StartLoc, NewLoc);

	const float HookTolerance = Context->GetHookTolerance();
	const float Distance = Context->GetStat().Distance;

	if (TargetDist < HookTolerance * HookTolerance)
		Context->SetState(Context->GetHookTarget() ? EHookState::Swing : EHookState::Idle);
	else if (HookDist > Distance * Distance)
		Context->SetState(EHookState::Idle);
}
