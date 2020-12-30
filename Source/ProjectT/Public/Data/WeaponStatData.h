#pragma once

#include "Data/ShotMode.h"
#include "WeaponStatData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct PROJECTT_API FWeaponStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ������"))
	float DamageInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ������"))
	float DistanceInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDamageDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ������"))
	float MaxDamageDistanceInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "EShotMode"))
	int32 ShotableMode = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "EShotMode"))
	EShotMode ShotMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float Speed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ������"))
	float SpeedInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float AimSpeed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ������"))
	float AimSpeedInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "��� 1ȸ �� ������"))
	float SpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1�� �� ���ҷ�"))
	float SpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	float MinSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	float MaxSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "��� 1ȸ �� �������� ���� �� ���ҷ�"))
	float SpreadIncDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1�ʰ� ���ҷ��� ���� �� ������"))
	float SpreadDecInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimMinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimMaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "��� 1ȸ �� ������"))
	float AimSpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1�� �� ������"))
	float AimSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	float AimMinSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	float AimMaxSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "��� 1ȸ �� �������� ���� �� ���ҷ�"))
	float AimSpreadIncDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1�ʰ� ���ҷ��� ���� �� ������"))
	float AimSpreadDecInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MinRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MaxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	FVector2D MinRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	FVector2D MaxRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D AimMinRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D AimMaxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	FVector2D AimMinRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	FVector2D AimMaxRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "���� �� ���ҷ�"))
	float ReloadTimeDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 MaxAmmoInMag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 MaxAmmoInPack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 BulletInShot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 BulletInBurst;
};
