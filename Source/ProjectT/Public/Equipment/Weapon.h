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

	void UseSkill(uint8 Index);
	void Reload();

private:
	void BeginPlay() override;

	void SetOwner(AActor* NewOwner) override;
	void OnRep_Owner() override;

	void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray
		<FLifetimeProperty>& OutLifetimeProps) const override;

	bool ReplicateSubobjects(UActorChannel* Channel,
		FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION()
	void OnRep_Context();

private:
	UPROPERTY(Transient)
	TArray<class USkill*> Skills;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Context)
	class UWeaponContext* Context;

	uint8 bInitContext : 1;
	uint8 bInit : 1;
};
