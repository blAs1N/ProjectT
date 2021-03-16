// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/MoveState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/Hook/HookStat.h"
#include "Equipment/Hook.h"
#include "Equipment/HookContext.h"

void FMoveState::Enter(UHookContext* Context)
{
	const auto Target = Context->GetTarget();
	StartLoc = Target->GetActorLocation();
	TimeElapsed = 0.0f;
}

void FMoveState::Tick(UHookContext* Context, float DeltaSeconds)
{
	const float MoveTolerance = Context->GetMoveTolerance();
	const auto Target = Context->GetTarget();

	const FVector TargetLoc = Target->GetActorLocation();
	const FVector HookLoc = Context->GetHookLocation();
	
	const auto& Stat = Context->GetStat();
	bool bComplete = FVector::DistSquared(Target->
		GetActorLocation(), HookLoc) <= MoveTolerance * MoveTolerance;
		
	bComplete = bComplete || TimeElapsed >= Stat.MaxMoveDuration;

	if (bComplete)
	{
		Context->SetState(EHookState::Idle);
		return;
	}

	FVector Power = (HookLoc - TargetLoc) * Context->GetStat().MoveScale *  DeltaSeconds;
	const float Speed = FMath::Clamp(Power.Size(), Stat.MinMoveSpeed, Stat.MaxMoveSpeed);
	Power.Normalize();
	
	Target->LaunchCharacter(Power * Speed, true, true);
	TimeElapsed += DeltaSeconds;
}

void FMoveState::Exit(UHookContext* Context)
{
	const auto Target = Context->GetTarget();
	const FVector Dir = Target->GetActorLocation() - StartLoc;
	Target->LaunchCharacter(Dir * Context->GetStat().EndMoveLaunchPower, true, true);
}
