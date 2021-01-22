// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTPlayerController.h"

void APTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OnPossessed.Broadcast(InPawn);
}

void APTPlayerController::OnUnPossess()
{
	OnUnPossessed.Broadcast(GetPawn());
	Super::OnUnPossess();
}