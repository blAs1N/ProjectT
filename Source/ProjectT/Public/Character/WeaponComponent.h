// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Data/WeaponStatData.h"
#include "WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchAim, bool, bSetAim);

UCLASS()
class PROJECTT_API UWeaponComponent final : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UWeaponComponent();

	void Initialize(const struct FWeaponData& InWeaponData);

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void StartAim();

	UFUNCTION(BlueprintCallable)
	void StopAim();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void SetShotMode(EShotMode NewShotMode);

	void LevelUp(uint8 LevelInc);


	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "ShotableMode"))
	FORCEINLINE int32 GetShotableModes() const noexcept { return Stat.ShotableMode; }

	FORCEINLINE EShotMode GetShotMode() const noexcept { return ShotMode; }
	FORCEINLINE uint8 GetClip() const noexcept { return Stat.Clip; }
	FORCEINLINE uint8 GetAmmo() const noexcept { return Stat.Ammo; }

	FORCEINLINE bool IsFiring() const noexcept { return bFiring; }
	FORCEINLINE bool IsAiming() const noexcept { return bAiming; }
	FORCEINLINE bool IsReloading() const noexcept { return bReloading; }

private:
	void BeginPlay() override;

	void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAim();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopAim();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetShotMode(EShotMode NewShotMode);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastReload();

	UFUNCTION(Client, Reliable)
	void ClientReload();

	FORCEINLINE bool ServerStartFire_Validate() const noexcept { return !bFiring; }
	FORCEINLINE bool ServerStopFire_Validate() const noexcept { return true; }

	FORCEINLINE void ServerStartAim_Implementation() { SetAimData(); }
	FORCEINLINE bool ServerStartAim_Validate() const noexcept { return true; }

	FORCEINLINE void ServerStopAim_Implementation() { SetUnaimData(); }
	FORCEINLINE bool ServerStopAim_Validate() const noexcept { return true; }

	FORCEINLINE void ServerReload_Implementation() { return MulticastReload(); }
	FORCEINLINE bool ServerReload_Validate() const noexcept { return true; }

	void ServerSetShotMode_Implementation(EShotMode NewShotMode) { ShotMode = NewShotMode; }
	FORCEINLINE bool ServerSetShotMode_Validate(EShotMode NewShotMode) const noexcept { return true; }

	FORCEINLINE void ClientReload_Implementation() { Reload(); }

	void Shot();
	void ReloadImpl();

	void SetAimData();
	void SetUnaimData();

public:
	FOnSwitchAim OnSwitchAim; // Client-only Delegate

private:
	FWeaponStatData Stat;
	
	UPROPERTY(Transient)
	class APTCharacter* Owner;

	UPROPERTY(Transient)
	UAnimMontage* ReloadAnim;

	UPROPERTY(EditAnywhere, meta = (AllowPrivate = true))
	FCollisionProfileName BulletCollisionProfile;

	FVector2D MinRecoil;
	FVector2D MaxRecoil;

	float MinSpread;
	float MaxSpread;
	float SpreadInc;
	float SpreadDec;

	float Delay;
	float AdditionalDmg;
	float FireLag;
	float Spread;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EShotMode ShotMode;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 Clip;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 Ammo;

	int32 ShotNum;

	UPROPERTY(Replicated, Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bFiring : 1;

	UPROPERTY(Replicated, Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bAiming : 1;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bReloading : 1;
};
