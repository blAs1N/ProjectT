// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MISC/NetworkObject.h"
#include "Data/Hook/HookStat.h"
#include "Data/Hook/HookState.h"
#include "HookContext.generated.h"

UCLASS()
class PROJECTT_API UHookContext final : public UNetworkObject
{
	GENERATED_BODY()
	
public:
	void Initialize(const struct FHookContextParam& Param);

	void TraceHookTarget();
	void SetState(EHookState NewState);
	void SetVisibility(bool bNewVisibility);

	FVector GetHookLocation() const;
	FVector GetHandLocation() const;

	FORCEINLINE void SetLength(float NewLength) { MulticastSetLength(NewLength); }

	FORCEINLINE class AHook* GetHook() const noexcept { return Hook; }
	FORCEINLINE class ACharacter* GetTarget() const noexcept { return Target; }

	FORCEINLINE class UPrimitiveComponent* GetHookTarget() const noexcept { return HookTarget; }
	FORCEINLINE FRotator GetHookRotation() const noexcept { return HookRot; }

	FORCEINLINE float GetHookTolerance() const noexcept { return HookTolerance; }
	FORCEINLINE float GetMoveTolerance() const noexcept { return MoveTolerance; }
	FORCEINLINE float GetLength() const noexcept { return Length; }

	FORCEINLINE const FHookStat& GetStat() const noexcept { return Stat; }

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetLength(float NewLength);

private:
	UPROPERTY(Transient)
	AHook* Hook;

	UPROPERTY(Transient)
	ACharacter* Target;

	UPROPERTY(Transient)
	UPrimitiveComponent* HookTarget;

	FName CollisionProfile;
	FName HandSocket;

	FVector FirstTargetLoc;
	FVector HookLoc;
	FRotator HookRot;

	FHookStat Stat;

	float HookTolerance;
	float MoveTolerance;
	float Length;
};
