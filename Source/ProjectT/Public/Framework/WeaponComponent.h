// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/WeaponStatData.h"
#include "WeaponComponent.generated.h"

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
	void SetShootMode(EShootMode NewShootMode);

	void LevelUp(uint8 LevelInc);

	int32 GetShootableModes() const noexcept { return Stat.ShootableMode; }

private:
	void BeginPlay() override;

	void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

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
	void ServerSetShootMode(EShootMode NewShootMode);

	void ServerStartFire_Implementation();
	FORCEINLINE bool ServerStartFire_Validate() const noexcept { return !bFiring; }

	void ServerStopFire_Implementation();
	FORCEINLINE bool ServerStopFire_Validate() const noexcept { return true; }

	void ServerStartAim_Implementation();
	FORCEINLINE bool ServerStartAim_Validate() const noexcept { return true; }

	void ServerStopAim_Implementation();
	FORCEINLINE bool ServerStopAim_Validate() const noexcept { return true; }

	void ServerReload_Implementation();
	FORCEINLINE bool ServerReload_Validate() const noexcept { return true; }

	void ServerSetShootMode_Implementation(EShootMode NewShootMode);
	FORCEINLINE bool ServerSetShootMode_Validate(EShootMode NewShootMode) const noexcept { return true; }

	void Shoot();

private:
	FWeaponStatData Stat;

	FVector2D MinRecoil;
	FVector2D MaxRecoil;

	float MinSpread;
	float MaxSpread;
	float SpreadInc;
	float SpreadDec;
	float Speed;

	float AdditionalDmg;
	float FireLag;
	float Spread;

	uint8 bAiming : 1;
	uint8 bFiring : 1;
};
