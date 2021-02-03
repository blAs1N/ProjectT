#pragma once

#include "UObject/ObjectMacros.h"
#include "WeaponStat.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FWeaponStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDamageDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float Speed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "��� 1ȸ �� ������"))
	float SpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1�� �� ���ҷ�"))
	float SpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MinRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MaxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BulletInShot;
};
