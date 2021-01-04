#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "StatData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToopTip = "���� �� ������"))
	float HealthInc;
};