// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/PostureState.h"
#include "PostureComponent.generated.h"

UCLASS()
class PROJECTT_API UPostureComponent final : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPostureComponent();
	void Initialize(const struct FPostureData* InPostureData);

	UFUNCTION(BlueprintCallable)
	void SetPosture(EPostureState NewState);

	UFUNCTION(BlueprintCallable)
	void SetSprint(bool bIsSprint);

	FORCEINLINE EPostureState GetPostureState() const noexcept { return State; }
	FORCEINLINE bool IsSprinting() const noexcept { return bIsSprinting; }

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPosture(EPostureState NewState);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprint(bool bIsSprint);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetPosture(EPostureState NewState);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetSprint(bool bIsSprint);

	FORCEINLINE void ServerSetPosture_Implementation(EPostureState NewState) { MulticastSetPosture(NewState); }
	FORCEINLINE bool ServerSetPosture_Validate(EPostureState NewState) const noexcept { return true; }

	FORCEINLINE void ServerSetSprint_Implementation(bool bIsSprint) { MulticastSetSprint(bIsSprint); }
	FORCEINLINE bool ServerSetSprint_Validate(bool bIsSprint) const noexcept { return true; }

	void MulticastSetPosture_Implementation(EPostureState NewState);
	void MulticastSetSprint_Implementation(bool bIsSprint);

	void SetPostureImpl(EPostureState NewState);
	void SetSprintImpl(bool bIsSprint);

	const FPostureData& GetPostureData() const noexcept;

private:
	UPROPERTY(Transient)
	class ACharacter* CharacterOwner;

	UPROPERTY(Transient)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(Transient)
	class UCharacterMovementComponent* MovementComp;

	const FPostureData* PostureData;

	FTimerHandle DelayTimer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float PostureSwitchDelay;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float ProneSwitchDelay;

	float WalkSpeed;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EPostureState State = EPostureState::Stand;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsSprinting : 1;

	uint8 bSwitchDelaying : 1;
};
