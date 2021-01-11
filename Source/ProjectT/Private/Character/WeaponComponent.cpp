// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/WeaponComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Character/PostureComponent.h"
#include "Character/PTCharacter.h"
#include "Data/WeaponData.h"
#include "Library/PTStatics.h"

UWeaponComponent::UWeaponComponent()
{
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::Initialize(const FWeaponData& WeaponData)
{
	Owner = Cast<APTCharacter>(GetOwner());

	Stat = WeaponData.Stat;
	ReloadAnim = WeaponData.Reload;

	Spread = Stat.MinSpread;
	ShotMode = Stat.ShotMode;
	
	Clip = Stat.Clip;
	Ammo = Stat.Ammo - Clip;
	
	check(Stat.ShotableMode & (1 << static_cast<uint8>(ShotMode)));

	FPTStatics::AsyncLoad(WeaponData.Mesh, [this, WeaponData]
	{
		SetSkeletalMesh(WeaponData.Mesh.Get());
	});

	SetRelativeTransform(WeaponData.Transform);
	AttachToComponent(GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform, WeaponData.Socket);
}

void UWeaponComponent::StartFire()
{
	check(Owner->IsLocallyControlled());
	if (bFiring || bReloading || Clip <= 0) return;
	
	if (!Owner->HasAuthority())
		bFiring = true;
	
	Owner->GetPostureComp()->SetSprint(false);
	ServerStartFire();
}

void UWeaponComponent::StopFire()
{
	check(Owner->IsLocallyControlled());
	if (!bFiring || ShotMode != EShotMode::FullAuto) return;
	
	if (!Owner->HasAuthority())
		bFiring = false;
	
	ServerStopFire();
}

void UWeaponComponent::StartAim()
{
	check(Owner->IsLocallyControlled());
	if (bAiming) return;

	if (!Owner->HasAuthority())
		bAiming = true;
	
	ServerStartAim();
	OnSwitchAim.Broadcast(true);
}

void UWeaponComponent::StopAim()
{
	check(Owner->IsLocallyControlled());
	if (!bAiming) return;
	
	if (!Owner->HasAuthority())
		bAiming = false;
	
	ServerStopAim();
	OnSwitchAim.Broadcast(false);
}

void UWeaponComponent::Reload()
{
	check(Owner->IsLocallyControlled());
	if (bReloading) return;

	ReloadImpl();
	ServerReload();
}

void UWeaponComponent::SetShotMode(EShotMode NewShotMode)
{
	check(Owner->IsLocallyControlled());

	if (ShotMode != NewShotMode && Stat.ShotableMode & (1 << static_cast<uint8>(NewShotMode)))
	{
		ShotMode = NewShotMode;

		if (!Owner->HasAuthority())
			ServerSetShotMode(NewShotMode);
	}
}

void UWeaponComponent::SetNextShotMode()
{
	uint8 NewShotMode = static_cast<uint8>(ShotMode) + 1;
	while (!(Stat.ShotableMode & (1 << static_cast<uint8>(NewShotMode))))
		NewShotMode = (NewShotMode + 1) % static_cast<uint8>(EShotMode::Max);
	SetShotMode(static_cast<EShotMode>(NewShotMode));
}

void UWeaponComponent::LevelUp(uint8 LevelInc)
{
	check(Owner->HasAuthority());

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

	bAiming ? SetAimData() : SetUnaimData();
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Owner->HasAuthority())
		SetUnaimData();
}

void UWeaponComponent::TickComponent(float DeltaTime,
	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Owner->HasAuthority() || Delay <= 0.0f) return;

	if (bFiring)
	{
		for (; FireLag <= 0; FireLag += Delay)
			Shot();

		FireLag -= DeltaTime;
	}
	else
	{
		Spread = FMath::Max(Spread - (Stat.SpreadDec * DeltaTime), MinSpread);
		FireLag = FMath::Max(FireLag - DeltaTime, 0.0f);
	}
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UWeaponComponent, bFiring, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UWeaponComponent, bAiming, COND_SkipOwner);
}

void UWeaponComponent::ServerStartFire_Implementation()
{
	const int32 MaxShot = Stat.Clip / Stat.BulletInShot;
	const int32 LeftBullets[3]{ 1, FMath::Min(Stat.ShotInBurst, MaxShot), MaxShot };
	ShotNum = LeftBullets[static_cast<uint8>(ShotMode)];
	bFiring = true;
}

void UWeaponComponent::ServerStopFire_Implementation()
{
	bFiring = false;
}

void UWeaponComponent::MulticastReload_Implementation()
{
	if (!Owner->IsLocallyControlled())
		ReloadImpl();
}

void UWeaponComponent::Shot()
{
	FVector Dir = FRotationMatrix{ Owner->GetControlRotation() }.GetScaledAxis(EAxis::X);
	Dir = FMath::VRandCone(Dir, Spread);

	const FVector Start = Owner->GetPawnViewLocation();
	const FVector End = Start + (Dir * Stat.Distance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	FHitResult Result;
	if (GetWorld()->LineTraceSingleByProfile(Result, Start, End, BulletCollisionProfile.Name, Params))
	{
		if (Cast<APTCharacter>(Result.GetActor()))
		{
			const FVector2D InputRange{ Stat.MaxDamageDistance, Stat.Distance };
			const FVector2D OutputRange{ Stat.MaxDamage, Stat.MinDamage };
			const float Dist = FVector::Distance(Start, Result.ImpactPoint);
			const float Damage = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, Dist) + AdditionalDmg;

			FPointDamageEvent DmgEvent{ Damage, Result, Dir, UDamageType::StaticClass() };
			Result.GetActor()->TakeDamage(Damage, DmgEvent, Owner->GetController(), Owner);
		}
	}

	Spread = FMath::Max(Spread + Stat.SpreadInc, MaxSpread);

	Stat.Clip -= Stat.BulletInShot;
	UE_LOG(LogTemp, Log, TEXT("Clip : %d"), Stat.Clip);
	if (--ShotNum > 0) return;

	bFiring = false;
	if (Stat.Clip <= 0) ClientReload();
}

void UWeaponComponent::ReloadImpl()
{
	const auto AddBullet = [this]
	{
		const int32 AddedBullet = FMath::Min(Stat.Clip - Clip, Ammo);
		Ammo -= AddedBullet;
		Clip += AddedBullet;
	};

	if (ReloadAnim && ReloadAnim->SequenceLength > 0.0f)
	{
		bReloading = true;
		const float PlayRate = Stat.ReloadTime / ReloadAnim->SequenceLength;
		Owner->PlayAnimMontage(ReloadAnim, PlayRate);

		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, [this, AddBullet]
			{
				AddBullet();
				bReloading = false;
			}, Stat.ReloadTime, false);
	}
	else AddBullet();
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