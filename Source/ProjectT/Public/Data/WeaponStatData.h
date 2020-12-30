#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "WeaponStatData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FWeaponStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ������"))
	float DamageInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDamageDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float Speed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float ZoomSpeed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "��� 1ȸ �� ������"))
	float SpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "1�� �� ���ҷ�"))
	float SpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomMinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomMaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "��� 1ȸ �� ������"))
	float ZoomSpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "1�� �� ������"))
	float ZoomSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 BulletSize;
};
