// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/HookStat.h"
#include "Equipment/State/StateBase.h"
#include "Hook.generated.h"

enum class EHookState : uint8
{
	Idle, Throw, Swing, Move
};

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

	void TraceHookTarget();
	void SetState(EHookState NewState);

	FVector GetHookLocation() const;
	FVector GetHandLocation() const;

	FORCEINLINE void SetCollision(bool bEnableCollision) { MulticastSetCollision(bEnableCollision); }
	FORCEINLINE void SetVisibility(bool bNewVisibility) { MulticastSetVisibility(bNewVisibility); }
	FORCEINLINE void SetLength(float NewLength) { MulticastSetLength(NewLength); }

	FORCEINLINE class UPrimitiveComponent* GetHookedTarget() const noexcept { return HookedTarget; }
	FORCEINLINE FRotator GetHookRotation() const noexcept { return HookRot; }

	FORCEINLINE float GetHookTolerance() const noexcept { return HookTolerance; }
	FORCEINLINE float GetMoveTolerance() const noexcept { return MoveTolerance; }
	FORCEINLINE float GetLength() const noexcept { return Length; }

	FORCEINLINE const FHookStat& GetStat() const noexcept { return Stat; }

private:
	void BeginPlay() override;

	void SetOwner(AActor* NewOwner) override;
	void OnRep_Owner() override;
	void Tick(float DeltaSeconds) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetCollision(bool bEnableCollision);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetVisibility(bool bNewVisibility);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetLength(float NewLength);

	void LoadAssets(const FHookData& Data, bool bLoadAsync);
	void AllocateState();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* HookMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UCableComponent* Cable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FName CollisionProfile;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FName EndPointSocket;

	TArray<TUniquePtr<class FStateBase>> States;

	UPROPERTY(Transient)
	UPrimitiveComponent* HookedTarget;

	FName HandSocket;

	FVector FirstTargetLoc;
	FVector HookLoc;
	FRotator HookRot;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float HookTolerance;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MoveTolerance;

	FHookStat Stat;
	float Length;

	EHookState State;
};
