#pragma once

#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "Data/CompositeModelParam.h"
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
	TSoftObjectPtr<class USoundBase> ThrowSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> PullSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform HookTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HandSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.1))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Thickness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BoostPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxBoostPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMoveDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PenetrationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EndMoveLaunchPower;
};
