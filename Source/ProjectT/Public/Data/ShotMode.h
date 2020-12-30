// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ShotMode.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class EShotMode : uint8
{
	Single,
	Burst,
	FullAuto,
};
