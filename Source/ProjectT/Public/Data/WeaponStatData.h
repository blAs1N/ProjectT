#pragma once

#include "Data/ShotMode.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ������"))
	float DistanceInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDamageDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ������"))
	float MaxDamageDistanceInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EShotMode"))
	int32 ShotableMode = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EShotMode"))
	EShotMode ShotMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float Speed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ������"))
	float SpeedInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float AimSpeed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ������"))
	float AimSpeedInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "��� 1ȸ �� ������"))
	float SpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "1�� �� ���ҷ�"))
	float SpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	float MinSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	float MaxSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "��� 1ȸ �� �������� ���� �� ���ҷ�"))
	float SpreadIncDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "1�ʰ� ���ҷ��� ���� �� ������"))
	float SpreadDecInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimMinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimMaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "��� 1ȸ �� ������"))
	float AimSpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "1�� �� ������"))
	float AimSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	float AimMinSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	float AimMaxSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "��� 1ȸ �� �������� ���� �� ���ҷ�"))
	float AimSpreadIncDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "1�ʰ� ���ҷ��� ���� �� ������"))
	float AimSpreadDecInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MinRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MaxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	FVector2D MinRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	FVector2D MaxRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D AimMinRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D AimMaxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	FVector2D AimMinRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	FVector2D AimMaxRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���� �� ���ҷ�"))
	float ReloadTimeDec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxAmmoInMag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxAmmoInPack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 BulletInShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 BulletInBurst;
};
