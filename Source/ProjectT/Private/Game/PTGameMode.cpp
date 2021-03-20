// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/PTGameMode.h"
#include "EngineUtils.h"
#include "MISC/DeadZone.h"

FVector APTGameMode::GetRandomZone() const
{
	if (!DeadZone)
		return FVector::ZeroVector;

	return FMath::RandPointInBox(DeadZone->GetSafeDeadBox());
}

bool APTGameMode::IsInsideZone(const FVector& Loc) const
{
	if (!DeadZone)
		return true;

	return DeadZone->GetDeadBox().IsInside(Loc);
}

void APTGameMode::BeginPlay()
{
	Super::BeginPlay();
	DeadZone = *TActorIterator<ADeadZone>{ GetWorld() };
}

