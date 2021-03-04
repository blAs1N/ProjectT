// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hook.generated.h"

enum class EHookState : uint8
{
	Idle, Throw, Hook, Move
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

private:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps
		(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void LoadAssets(const FHookData& Data, bool bLoadAsync);
	void TraceHookTarget();
	void CleanVariable();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* HookMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UCableComponent* Cable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FName CollisionProfile;

	UPROPERTY(Transient)
	class USoundBase* ThrowSound;

	UPROPERTY(Transient)
	class USoundBase* PullSound;

	UPROPERTY(Transient)
	class UPrimitiveComponent* HookedTarget;

	FVector FirstHookLocation;
	FVector HookLocation;
	FVector HookNormal;

	float Distance;
	float MaxMoveDuration;
	float PenetrationOffset;

	EHookState State;
};
