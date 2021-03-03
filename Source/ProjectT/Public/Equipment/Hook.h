// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hook.generated.h"

UCLASS()
class PROJECTT_API AHook final : public AActor
{
	GENERATED_BODY()
	
public:
	AHook();

	void Initialize(const struct FHookData& Data);

	FORCEINLINE void Hook() { ServerHook(); }
	FORCEINLINE void Unhook() { ServerUnhook(); }
	FORCEINLINE void MoveTo() { ServerMoveTo(); }

private:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps
		(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHook();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnhook();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveTo();

	void ServerHook_Implementation();
	void ServerUnhook_Implementation();
	void ServerMoveTo_Implementation();

	FORCEINLINE bool ServerHook_Validate() const noexcept { return true; }
	FORCEINLINE bool ServerUnhook_Validate() const noexcept { return true; }
	FORCEINLINE bool ServerMoveTo_Validate() const noexcept { return true; }

	void SetHookState(bool bIsHooked);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Hook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UCableComponent* Cable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAudioComponent* AudioComp;

	UPROPERTY(Transient)
	AActor* HookedTarget;

	FVector StartPoint;

	uint8 bHooked : 1;
	uint8 bMoving : 1;
	uint8 bThrowing : 1;
};
