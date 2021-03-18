// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PTGameState.generated.h"

UCLASS(BlueprintType)
class PROJECTT_API APTGameState final : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FVector GetRandomZone();

	UFUNCTION(BlueprintCallable)
	bool IsInsideZone(const FVector& Loc) const;

private:
	void BeginPlay() override;

	FBox GetDeadBox() const;

private:
	UPROPERTY(Transient)
	class ADeadZone* DeadZone;
};
