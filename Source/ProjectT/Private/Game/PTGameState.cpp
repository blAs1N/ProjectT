// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/PTGameState.h"
#include "EngineUtils.h"
#include "MISC/DeadZone.h"

FVector APTGameState::GetRandomZone() const
{
	if (!DeadZone)
		return FVector::ZeroVector;

	return FMath::RandPointInBox(DeadZone->GetDeadBox());
}

bool APTGameState::IsInsideZone(const FVector& Loc) const
{
	if (!DeadZone)
		return true;

	return DeadZone->GetDeadBox().IsInside(Loc);
}

void APTGameState::BeginPlay()
{
	Super::BeginPlay();	
	DeadZone = *TActorIterator<ADeadZone>{ GetWorld() };
}
