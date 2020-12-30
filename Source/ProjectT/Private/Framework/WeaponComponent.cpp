// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Data/WeaponData.h"
#include "Library/PTStatics.h"

UWeaponComponent::UWeaponComponent()
{
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::Initialize(const FWeaponData& WeaponData)
{
	Stat = WeaponData.Stat;
	Spread = Stat.MinSpread;
	check(Stat.ShotableMode & (1 << static_cast<uint8>(Stat.ShotMode)));

	FPTStatics::AsyncLoad(WeaponData.Mesh, [this, WeaponData]
	{
		SetSkeletalMesh(WeaponData.Mesh.Get());
	});

	SetRelativeTransform(WeaponData.Transform);
	AttachToComponent(GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform, WeaponData.Socket);
}

void UWeaponComponent::StartFire()
{
	ServerStartFire();
}

void UWeaponComponent::StopFire()
{
	ServerStopFire();
}

void UWeaponComponent::StartAim()
{
	ServerStartAim();
}

void UWeaponComponent::StopAim()
{
	ServerStopAim();
}

void UWeaponComponent::Reload()
{
	ServerReload();
}

void UWeaponComponent::SetShotMode(EShotMode ShotMode)
{
	if (Stat.ShotMode != ShotMode && Stat.ShotableMode & (1 << static_cast<uint8>(ShotMode)))
		ServerSetShotMode(ShotMode);
}

void UWeaponComponent::LevelUp(uint8 LevelInc)
{
	AdditionalDmg += Stat.DamageInc * LevelInc;
	Stat.Distance += Stat.DistanceInc * LevelInc;
	Stat.MaxDamageDistance += Stat.MaxDamageDistanceInc * LevelInc;

	Stat.Speed += Stat.SpeedInc * LevelInc;
	Stat.AimSpeed += Stat.AimSpeedInc * LevelInc;

	FVector2D SpreadDecVec;
	if (bAiming)
		SpreadDecVec.Set(Stat.AimMinSpreadDec, Stat.AimMaxSpreadDec);
	else
		SpreadDecVec.Set(Stat.MinSpreadDec, Stat.MaxSpreadDec);
	
	FVector2D PrevSpread{ MinSpread, MaxSpread };
	FVector2D CurSpread = PrevSpread - (SpreadDecVec * LevelInc);

	Spread = FMath::GetMappedRangeValueUnclamped(PrevSpread, CurSpread, Spread);

	Stat.MinSpread -= Stat.MinSpreadDec * LevelInc;
	Stat.MaxSpread -= Stat.MaxSpreadDec * LevelInc;
	Stat.SpreadInc -= Stat.SpreadIncDec * LevelInc;
	Stat.SpreadDec += Stat.SpreadDecInc * LevelInc;

	Stat.AimMinSpread -= Stat.AimMinSpreadDec * LevelInc;
	Stat.AimMaxSpread -= Stat.AimMaxSpreadDec * LevelInc;
	Stat.AimSpreadInc -= Stat.AimSpreadIncDec * LevelInc;
	Stat.AimSpreadDec += Stat.AimSpreadDecInc * LevelInc;

	Stat.MinRecoil -= Stat.MinRecoilDec * LevelInc;
	Stat.MaxRecoil -= Stat.MaxRecoilDec * LevelInc;

	Stat.AimMinRecoil -= Stat.AimMinRecoilDec * LevelInc;
	Stat.AimMaxRecoil -= Stat.AimMaxRecoilDec * LevelInc;

	Stat.ReloadTime -= Stat.ReloadTimeDec * LevelInc;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
		ServerStopAim_Implementation();
}

void UWeaponComponent::TickComponent(float DeltaTime,
	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!GetOwner()->HasAuthority() || Delay <= 0.0f) return;

	if (bFiring)
	{
		for (; FireLag <= 0; FireLag += Delay)
			Shot();

		FireLag += DeltaTime;
		return;
	}
	
	Spread = FMath::Max(Spread - (Stat.SpreadDec * DeltaTime), bAiming ? Stat.AimMinSpread : Stat.MinSpread);
}

void UWeaponComponent::ServerStartFire_Implementation()
{
	FireLag = 1.0f / Speed;
	bFiring = true;
}

void UWeaponComponent::ServerStopFire_Implementation()
{
	bFiring = false;
}

void UWeaponComponent::ServerStartAim_Implementation()
{
	SetAimData();
}

void UWeaponComponent::ServerStopAim_Implementation()
{
	SetUnaimData();
}

void UWeaponComponent::ServerReload_Implementation()
{
	ServerStopAim_Implementation();
	ServerStopFire_Implementation();
}

void UWeaponComponent::ServerSetShotMode_Implementation(EShotMode NewShotMode)
{
	Stat.ShotMode = NewShotMode;
}

void UWeaponComponent::Shot()
{
	UE_LOG(LogTemp, Log, TEXT("Shot!"));
	Spread = FMath::Max(Spread + Stat.SpreadInc, MaxSpread);

	if (Stat.ShotMode != EShotMode::FullAuto)
		if (--LeftBullet == 0)
			bFiring = false;
}

void UWeaponComponent::SetAimData()
{
	MinRecoil = Stat.AimMinRecoil;
	MaxRecoil = Stat.AimMaxRecoil;

	MinSpread = Stat.AimMinSpread;
	MaxSpread = Stat.AimMaxSpread;
	SpreadInc = Stat.AimSpreadInc;
	SpreadDec = Stat.AimSpreadDec;

	Delay = (Stat.AimSpeed > 0.0f) ? (1.0f / Stat.AimSpeed) : 0.0f;
	bAiming = true;
}

void UWeaponComponent::SetUnaimData()
{
	MinRecoil = Stat.MinRecoil;
	MaxRecoil = Stat.MaxRecoil;

	MinSpread = Stat.MinSpread;
	MaxSpread = Stat.MaxSpread;
	SpreadInc = Stat.SpreadInc;
	SpreadDec = Stat.SpreadDec;

	Delay = (Stat.Speed > 0.0f) ? (1.0f / Stat.Speed) : 0.0f;
	bAiming = false;
}