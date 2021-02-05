// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	void Initialize(int32 Key);

	/*UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void Reload();*/

	FORCEINLINE class USkeletalMeshComponent* GetMesh() const noexcept { return Mesh; }

	FORCEINLINE int32 GetClip() const noexcept { return Clip; }
	/*FORCEINLINE uint8 GetAmmo() const noexcept { return Stat.Ammo; }

	FORCEINLINE bool IsFiring() const noexcept { return bFiring; }*/

//private:
//	void BeginPlay() override;
//
//	void Tick(float DeltaTime) override;
//
//	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//
//	UFUNCTION(Server, Reliable, WithValidation)
//	void ServerStartFire();
//
//	UFUNCTION(Server, Reliable, WithValidation)
//	void ServerStopFire();
//
//	UFUNCTION(Server, Reliable, WithValidation)
//	void ServerReload();
//
//	UFUNCTION(NetMulticast, Reliable)
//	void MulticastReload();
//
//	FORCEINLINE bool ServerStartFire_Validate() const noexcept { return !bFiring; }
//	FORCEINLINE bool ServerStopFire_Validate() const noexcept { return bFiring; }
//
//	FORCEINLINE void ServerReload_Implementation() { return MulticastReload(); }
//	FORCEINLINE bool ServerReload_Validate() const noexcept { return true; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	class UDataTable* WeaponDataTable;

	const struct FWeaponStat* Stat;

	int32 Clip;
};
