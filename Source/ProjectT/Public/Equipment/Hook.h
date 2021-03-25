// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/Hook/HookStat.h"
#include "Data/Hook/HookState.h"
#include "Equipment/State/StateBase.h"
#include "Hook.generated.h"

UCLASS()
class PROJECTT_API AHook final : public AActor
{
	GENERATED_BODY()
	
public:
	AHook();

	void Initialize(const struct FHookData& Data, bool bLoadAsync);

	void Hook();
	void Unhook();
	void MoveTo();

	void SetState(EHookState NewState);

	FORCEINLINE class UStaticMeshComponent* GetHookMesh() const noexcept { return HookMesh; }
	FORCEINLINE class UCableComponent* GetCable() const noexcept { return Cable; }

private:
	void BeginPlay() override;

	void SetOwner(AActor* NewOwner) override;
	void OnRep_Owner() override;

	void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray
		<FLifetimeProperty>& OutLifetimeProps) const override;

	bool ReplicateSubobjects(UActorChannel* Channel,
		FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	void CreateContext();

	UFUNCTION()
	void OnRep_Context();

	void LoadAssets(const FHookData& Data, bool bLoadAsync);

private:
	TArray<TUniquePtr<class FStateBase>> States;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Context)
	class UHookContext* Context;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* HookMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UCableComponent* Cable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FName CollisionProfile;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FName EndPointSocket;

	FName HandSocket;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float HookTolerance;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MoveTolerance;

	FHookStat Stat;

	float Delay;
	float CurDelay;

	EHookState State : 2;
	uint8 bInit : 1;
};
