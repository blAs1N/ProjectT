// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/PTGameState.h"
#include "EngineUtils.h"
#include "MISC/DeadZone.h"

FVector APTGameState::GetRandomZone()
{
	return FMath::RandPointInBox(GetDeadBox());
}

bool APTGameState::IsInsideZone(const FVector& Loc) const
{
	return GetDeadBox().IsInside(Loc);
}

void APTGameState::BeginPlay()
{
	Super::BeginPlay();	
	DeadZone = *TActorIterator<ADeadZone>{ GetWorld() };
}

FBox APTGameState::GetDeadBox() const
{
	if (!DeadZone)
		return FBox{};

	const FVector Loc = DeadZone->GetActorLocation();
	const FVector Size = DeadZone->GetActorScale3D() * 50.0f;

	const FVector BoxMin = Loc - Size;
	const FVector BoxMax = Loc + Size;
	return FBox{ BoxMin, BoxMax };
}
