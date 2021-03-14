// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTT_API AWeapon final : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	void Initialize(const struct FWeaponData& Data, bool bLoadAsync);

	void BeginSkill(uint8 Index);
	void EndSkill(uint8 Index);

private:
	void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(Transient)
	TArray<class USkill*> Skills;

	uint8 bInit : 1;
};
