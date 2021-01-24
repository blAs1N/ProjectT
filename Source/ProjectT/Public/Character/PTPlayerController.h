// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PTPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossessed, APawn*, InPawn);

UCLASS()
class PROJECTT_API APTPlayerController final : public APlayerController
{
	GENERATED_BODY()
	
private:
	void SetPawn(APawn* InPawn) override;

public:
	FOnPossessed OnPossessed;
	FOnPossessed OnUnPossessed;
};
