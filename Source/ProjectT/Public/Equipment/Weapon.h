// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/Weapon/WeaponStat.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTT_API AWeapon final : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	void Initialize(const struct FWeaponData& Data, bool bLoadAsync);

	void BeginFire();
	void EndFire();

	void Reload();

private:
	void SetOwner(AActor* NewOwner) override;
	void OnRep_Owner() override;

	void Tick(float DeltaSeconds) override;

private:
	FWeaponStat Stat;
};
