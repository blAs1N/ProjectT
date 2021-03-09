// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Interface/Initializable.h"
#include "PTCharacter.generated.h"

UCLASS(config=Game)
class PROJECTT_API APTCharacter final : public ACharacter, public IInitializable
{
	GENERATED_BODY()

public:
	APTCharacter(const FObjectInitializer& ObjectInitializer);

	float TakeDamage(float Damage, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FVector GetPawnViewLocation() const override { return GetViewLocation(); }
	FORCEINLINE class UHookComponent* GetHookComponent() const noexcept { return HookComp; }
	FORCEINLINE class UWeaponComponent* GetWeaponComponent() const noexcept { return WeaponComp; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() const noexcept { return WeaponMeshComp; }

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FVector GetViewLocation() const;

private:
	void OnInitialize(int32 Key) override;
	void OnGetData(const struct FCharacterData& Data);

	FORCEINLINE FVector GetViewLocation_Implementation() const { return Super::GetPawnViewLocation(); }
	FORCEINLINE bool IsLoadAsync_Implementation() const noexcept override { return bLoadAsync; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = true))
	UHookComponent* HookComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = true))
	UWeaponComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UDataTable> CharacterDataTable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float BackConstant;

	float BackPercent;
	float Weight;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint8 bLoadAsync : 1;
};
