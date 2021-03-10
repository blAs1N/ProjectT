// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/MoveState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Equipment/Hook.h"

void FMoveState::Enter()
{
	const auto Target = GetOwner<AHook>()->GetOwner<ACharacter>();
	
	StartLoc = Target->GetActorLocation();
	Target->GetCharacterMovement()->
		SetMovementMode(EMovementMode::MOVE_Flying);
}

void FMoveState::Tick(float DeltaSeconds)
{
	const auto Hook = GetOwner<AHook>();
	const auto Target = Hook->GetOwner<ACharacter>();

	const float MoveTolerance = Hook->GetMoveTolerance();
	const FVector HookLoc = Hook->GetHookLocation();
	const bool bComplete = FVector::DistSquared(Target->
		GetActorLocation(), HookLoc) <= MoveTolerance * MoveTolerance;

	if (bComplete)
	{
		Hook->SetState(EHookState::Idle);
		return;
	}

	TimeElapsed += DeltaSeconds;

	const float MaxMoveDuration = Hook->GetStat().MaxMoveDuration;
	const float BlendPct = MaxMoveDuration > 0.0f ? TimeElapsed / MaxMoveDuration : 1.0f;
	const FVector NewLoc = bComplete ? HookLoc : FMath::Lerp(StartLoc, HookLoc, BlendPct);
	Target->SetActorLocation(NewLoc);
}

void FMoveState::Exit()
{
	const auto Hook = GetOwner<AHook>();
	const auto Target = Hook->GetOwner<ACharacter>();
	Target->GetCharacterMovement()->SetDefaultMovementMode();

	const FVector Dir = Target->GetActorLocation() - StartLoc;
	Target->LaunchCharacter(Dir * Hook->GetStat().EndMoveLaunchPower, true, true);

	TimeElapsed = 0.0f;
}
