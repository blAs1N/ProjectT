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

	FORCEINLINE class UWeaponComponent* GetWeaponComponent() const noexcept { return WeaponComp; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() const noexcept { return WeaponMeshComp; }

private:
	void PostInitializeComponents() override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void Initialize();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	UWeaponComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(EditAnywhere, Category = Data, meta = (AllowPrivateAccess = true))
	class UDataTable* CharacterDataTable;

	UPROPERTY(EditAnywhere, Category = Data, meta = (AllowPrivateAccess = true))
	uint32 CharacterKey;

	UPROPERTY(EditAnywhere, Category = System, meta = (AllowPrivateAccess = true))
	float BackConstant;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly, Category = System, meta = (AllowPrivateAccess = true))
	float BackPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = System, meta = (AllowPrivateAccess = true))
	float Weight;

	uint32 AppliedKey;
};
