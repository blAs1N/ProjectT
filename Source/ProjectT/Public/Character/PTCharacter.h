// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "PTCharacter.generated.h"

UCLASS(config=Game)
class PROJECTT_API APTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APTCharacter(const FObjectInitializer& ObjectInitializer);

	float TakeDamage(float Damage, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE class AWeapon* GetWeapon() const noexcept { return Weapon; }

private:
	void OnConstruction(const FTransform& Transform) override;

	void Initialize();

private:
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	class UDataTable* CharacterDataTable;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	int32 CharacterKey;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float BackConstant;

	float BackPercent;

	float Weight;

	int32 AppliedKey;
};
