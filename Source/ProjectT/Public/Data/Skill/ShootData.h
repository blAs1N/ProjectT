// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/Weapon/ShootStat.h"
#include "ShootData.generated.h"

UCLASS(BlueprintType)
class UShootData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FShootStat Stat;
};
