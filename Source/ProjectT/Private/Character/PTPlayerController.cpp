// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTPlayerController.h"

void APTPlayerController::SetPawn(APawn* InPawn)
{
	FOnPossessed* Callback = nullptr;
	APawn* Param = nullptr;

	if (InPawn)
	{
		Callback = &OnPossessed;
		Param = InPawn;
	}
	else
	{
		Callback = &OnUnPossessed;
		Param = InPawn;
	}

	Super::SetPawn(InPawn);
	Callback->Broadcast(Param);
}
