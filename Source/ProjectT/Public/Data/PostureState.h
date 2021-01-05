#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "PostureState.generated.h"

UENUM(BlueprintType)
enum class EPostureState : uint8
{
	Stand, Crouch, Prone
};