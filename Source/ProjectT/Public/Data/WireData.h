#pragma once

#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "Data/CompositeModelParam.h"
#include "WireData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FWireData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HandSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Distance;
};
