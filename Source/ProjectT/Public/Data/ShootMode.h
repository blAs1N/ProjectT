// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ShootMode.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class EShootMode : uint8
{
	Single,
	Burst,
	FullAuto,
};
