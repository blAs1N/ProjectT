// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/MoveState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/Hook/HookStat.h"
#include "Equipment/Hook.h"
#include "Equipment/HookContext.h"

void FMoveState::Enter(UHookContext* Context)
{
	TimeElapsed = 0.0f;

	Context->SetCollision(false);
	Context->SetMovementMode(EMovementMode::MOVE_Flying);

	const auto Target = Context->GetTarget();
	StartLoc = Target->GetActorLocation();
}

void FMoveState::Tick(UHookContext* Context, float DeltaSeconds)
{
	const float MaxMoveDuration = Context->GetStat().MaxMoveDuration;
	const float MoveTolerance = Context->GetMoveTolerance();
	const FVector HookLoc = Context->GetHookLocation();
	const auto Target = Context->GetTarget();
	
	bool bComplete = FVector::DistSquared(Target->
		GetActorLocation(), HookLoc) <= MoveTolerance * MoveTolerance;

	bComplete = bComplete || TimeElapsed >= MaxMoveDuration;

	if (bComplete)
	{
		Context->SetState(EHookState::Idle);
		return;
	}

	TimeElapsed += DeltaSeconds;

	const float BlendPct = MaxMoveDuration > 0.0f ? TimeElapsed / MaxMoveDuration : 1.0f;
	const FVector NewLoc = bComplete ? HookLoc : FMath::Lerp(StartLoc, HookLoc, BlendPct);
	Target->SetActorLocation(NewLoc);
}

void FMoveState::Exit(UHookContext* Context)
{
	Context->SetCollision(true);
	Context->SetMovementMode();

	const auto Target = Context->GetTarget();
	const FVector Dir = Target->GetActorLocation() - StartLoc;
	Target->LaunchCharacter(Dir * Context->GetStat().EndMoveLaunchPower, true, true);
}
