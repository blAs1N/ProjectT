// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Initializable.h"
#include "WeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTT_API UWeaponComponent final : public UActorComponent, public IInitializable
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

	UFUNCTION(BlueprintCallable)
	void BeginFire();

	UFUNCTION(BlueprintCallable)
	void EndFire();

	UFUNCTION(BlueprintCallable)
	void Reload();

private:
	void BeginPlay() override;

	void OnInitialize(int32 InKey) override;
	void OnGetData(const struct FWeaponData& Data);

	void GetLifetimeReplicatedProps(TArray
		<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Weapon();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	FORCEINLINE bool ServerBeginFire_Validate() const noexcept { return true; }
	FORCEINLINE bool ServerEndFire_Validate() const noexcept { return true; }
	FORCEINLINE bool ServerReload_Validate() const noexcept { return true; }

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<class UDataTable> WeaponDataTable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Weapon)
	AWeapon* WeaponInst;

	int32 Key;
};
