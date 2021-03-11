// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/MoveState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Equipment/Hook.h"
#include "Kismet/KismetSystemLibrary.h"

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

	const float MaxMoveDuration = Hook->GetStat().MaxMoveDuration;
	const float MoveTolerance = Hook->GetMoveTolerance();
	const FVector HookLoc = Hook->GetHookLocation();
	bool bComplete = FVector::DistSquared(Target->
		GetActorLocation(), HookLoc) <= MoveTolerance * MoveTolerance;

	bComplete = bComplete || TimeElapsed >= MaxMoveDuration;

	if (bComplete)
	{
		Hook->SetState(EHookState::Idle);
		return;
	}

	TimeElapsed += DeltaSeconds;

	const float BlendPct = MaxMoveDuration > 0.0f ? TimeElapsed / MaxMoveDuration : 1.0f;
	FVector NewLoc = bComplete ? HookLoc : FMath::Lerp(StartLoc, HookLoc, BlendPct);
	
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(Hook);
	Param.AddIgnoredActor(Target);

	const FVector MeshLoc = Target->GetMesh()->GetComponentLocation();
	const FVector DiffZ = FVector{ 0.0f, 0.0f, Target->GetActorLocation().Z - MeshLoc.Z };
	const auto World = Hook->GetWorld();

	TArray<AActor*> Ignores{ Hook, Target };

	FHitResult Result;
	const bool bHit = UKismetSystemLibrary::LineTraceSingle(World, MeshLoc, NewLoc - DiffZ,
		ETraceTypeQuery::TraceTypeQuery1, false, Ignores, EDrawDebugTrace::None, Result, false);
	/*const bool bHit = World->LineTraceSingleByChannel(Result,
		MeshLoc, NewLoc - DiffZ, ECollisionChannel::ECC_Visibility, Param);*/

	if (bHit)
	{
		UKismetSystemLibrary::LineTraceSingle(World, NewLoc - DiffZ, NewLoc - DiffZ + FVector{ 0.0f, 0.0f, 100.0f },
			ETraceTypeQuery::TraceTypeQuery1, false, Ignores, EDrawDebugTrace::ForDuration, Result, false);

		World->LineTraceSingleByChannel(Result, NewLoc - DiffZ, NewLoc - DiffZ +
			FVector{ 0.0f, 0.0f, 100.0f }, ECollisionChannel::ECC_Visibility, Param);

		
		NewLoc = Result.Location + DiffZ;
	}

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
