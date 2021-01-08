// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Data/StatData.h"
#include "PTCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(config=Game)
class PROJECTT_API APTCharacter final : public ACharacter
{
	GENERATED_BODY()

public:
	APTCharacter();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Heal(float Value);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddExp(float Value);

	FORCEINLINE class UWeaponComponent* GetWeaponComp() const noexcept { return WeaponComp; }
	FORCEINLINE class UPostureComponent* GetPostureComp() const noexcept { return PostureComp; }
	FORCEINLINE bool IsDeath() const noexcept { return bIsDeath; }

private:
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void PostInitializeComponents() override;
	void BeginPlay() override;

	float TakeDamage(float Damage, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	FVector GetPawnViewLocation() const override;

	void Initialize();
	void Death();

public:
	FOnDeath OnDeath;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UWeaponComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UPostureComponent* PostureComp;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FDataTableRowHandle CharacterKey;

	FStatData Stat;

	float Health;
	float CurExp;
	float CurMaxExp;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MaxExp;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float ExpIncValue;

	uint8 Level;
	uint8 bIsDeath : 1;
};
