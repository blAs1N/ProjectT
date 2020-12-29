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

	void Reload();

private:
	void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopFire();

	void ServerStartFire_Implementation() noexcept;
	FORCEINLINE bool ServerStartFire_Validate() const noexcept { return !bFiring; }

	FORCEINLINE void ServerStopFire_Implementation() noexcept { bFiring = false; }
	FORCEINLINE bool ServerStopFire_Validate() const noexcept { return bFiring; }

	void Shoot();

private:
	FWeaponStatData Stat;
	float FireLag;
	float Spread;
	uint8 bZooming : 1;
	uint8 bFiring : 1;
};
