// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UCLASS (ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTT_API UWeaponComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

	void Initialize(uint32 Key);

//	UFUNCTION(BlueprintCallable)
//	void StartFire();
//
//	UFUNCTION(BlueprintCallable)
//	void StopFire();
//
//	UFUNCTION(BlueprintCallable)
//	void Reload();
//
//	FORCEINLINE int32 GetClip() const noexcept { return Clip; }
//	FORCEINLINE uint8 GetAmmo() const noexcept { return Stat.Ammo; }
//
//	FORCEINLINE bool IsFiring() const noexcept { return bFiring; }
//
//private:
//	void BeginPlay() override;
//
//	void Tick(float DeltaTime) override;
//
//	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//
//	UFUNCTION(Server, Reliable, WithValidation)
//		void ServerStartFire();
//
//	UFUNCTION(Server, Reliable, WithValidation)
//		void ServerStopFire();
//
//	UFUNCTION(Server, Reliable, WithValidation)
//		void ServerReload();
//
//	UFUNCTION(NetMulticast, Reliable)
//		void MulticastReload();
//
//	FORCEINLINE bool ServerStartFire_Validate() const noexcept { return !bFiring; }
//	FORCEINLINE bool ServerStopFire_Validate() const noexcept { return bFiring; }
//
//	FORCEINLINE void ServerReload_Implementation() { return MulticastReload(); }
//	FORCEINLINE bool ServerReload_Validate() const noexcept { return true; }

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	class UDataTable* WeaponDataTable;

	const struct FWeaponStat* Stat;

	uint32 Clip;
};
