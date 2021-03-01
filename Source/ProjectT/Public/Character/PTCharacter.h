// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Interface/Loadable.h"
#include "PTCharacter.generated.h"

UCLASS(config=Game)
class PROJECTT_API APTCharacter final : public ACharacter, public ILoadable
{
	GENERATED_BODY()

public:
	APTCharacter(const FObjectInitializer& ObjectInitializer);

	float TakeDamage(float Damage, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE class UWireComponent* GetWireComponent() const noexcept { return WireComp; }
	FORCEINLINE class UWeaponComponent* GetWeaponComponent() const noexcept { return WeaponComp; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() const noexcept { return WeaponMeshComp; }

private:
	void PostActorCreated() override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void Initialize();

	void OnLoadDataTable(const TSoftObjectPtr<class UDataTable>& DataTable);

	bool IsLoadAsync_Implementation() const noexcept { return bLoadAsync; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = true))
	UWireComponent* WireComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = true))
	UWeaponComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(EditAnywhere, Category = Data, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UDataTable> CharacterDataTable;

	UPROPERTY(EditAnywhere, Category = Data, meta = (AllowPrivateAccess = true))
	uint32 CharacterKey;

	UPROPERTY(EditAnywhere, Category = System, meta = (AllowPrivateAccess = true))
	float BackConstant;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly, Category = System, meta = (AllowPrivateAccess = true))
	float BackPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = System, meta = (AllowPrivateAccess = true))
	float Weight;

	uint8 bLoadingAsset : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = true))
	uint8 bLoadAsync : 1;
};
