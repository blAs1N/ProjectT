#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "StatData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToopTip = "레벨 당 증가량"))
	float HealthInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrawlSpeed;
};