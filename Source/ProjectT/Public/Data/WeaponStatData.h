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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 증가량"))
	float DamageInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 증가량"))
	float DistanceInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDamageDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 증가량"))
	float MaxDamageDistanceInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "EShotMode"))
	int32 ShotableMode = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "EShotMode"))
	EShotMode ShotMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float Speed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 증가량"))
	float SpeedInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float AimSpeed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 증가량"))
	float AimSpeedInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "사격 1회 당 증가량"))
	float SpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1초 간 감소량"))
	float SpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	float MinSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	float MaxSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "사격 1회 당 증가량의 레벨 당 감소량"))
	float SpreadIncDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1초간 감소량의 레벨 당 증가량"))
	float SpreadDecInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimMinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimMaxSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "사격 1회 당 증가량"))
	float AimSpreadInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1초 간 증가량"))
	float AimSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	float AimMinSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	float AimMaxSpreadDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "사격 1회 당 증가량의 레벨 당 감소량"))
	float AimSpreadIncDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "1초간 감소량의 레벨 당 증가량"))
	float AimSpreadDecInc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MinRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MaxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	FVector2D MinRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	FVector2D MaxRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D AimMinRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D AimMaxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	FVector2D AimMinRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	FVector2D AimMaxRecoilDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "레벨 당 감소량"))
	float ReloadTimeDec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Clip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 BulletInShot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 ShotInBurst;
};
