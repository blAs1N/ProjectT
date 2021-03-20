// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PTGameMode.generated.h"

UCLASS()
class PROJECTT_API APTGameMode final : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetRandomZone() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInsideZone(const FVector& Loc) const;

private:
	void BeginPlay() override;

private:
	UPROPERTY(Transient)
	class ADeadZone* DeadZone;
};
