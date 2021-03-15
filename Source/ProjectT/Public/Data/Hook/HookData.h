#pragma once

#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "Data/Hook/HookStat.h"
#include "HookData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FHookData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UStaticMesh> HookMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> CableMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform HookTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HandSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Thickness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Delay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHookStat Stat;
};
