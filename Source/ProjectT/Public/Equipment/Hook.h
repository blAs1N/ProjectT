// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

private:
	void PostActorCreated() override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray
		<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetVisibility(bool bNewVisibility);

	UFUNCTION()
	void OnRep_Length();

	void TickThrow(float DeltaSeconds);
	void TickSwing(float DeltaSeconds);
	void TickMove(float DeltaSeconds);

	void EndThrow(bool bSuccess);

	void LoadAssets(const FHookData& Data, bool bLoadAsync);
	void TraceHookTarget();
	void ApplyProperty();
	void Clear();

	FVector GetHandLoc() const;
	FVector GetOffset() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* HookMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UCableComponent* Cable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FName CollisionProfile;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FName EndPointSocket;

	UPROPERTY(Transient)
	class USoundBase* ThrowSound;

	UPROPERTY(Transient)
	class USoundBase* PullSound;

	UPROPERTY(Transient)
	class UPrimitiveComponent* HookedTarget;

	FName HandSocket;

	FVector FirstHookLoc;
	FVector StartLoc;
	FVector HookLoc;

	FRotator HookRot;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float HookTolerance;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MoveTolerance;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Length)
	float Length;

	float TimeElapsed;

	float Speed;
	float Distance;
	float BoostPower;
	float MaxBoostPower;
	float MaxMoveDuration;
	float PenetrationOffset;
	float EndMoveLaunchPower;
	
	EHookState State;
};
