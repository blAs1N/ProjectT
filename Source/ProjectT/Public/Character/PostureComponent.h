// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/PostureState.h"
#include "PostureComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchPosture, EPostureState, NewState);

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
	FORCEINLINE bool IsPostureSwitching() const noexcept { return bPostureSwitching; }
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

	void SetPostureImpl(EPostureState NewState);
	void SetSprintImpl(bool bIsSprint);

	void SetPostureData(EPostureState NewState);

	const FPostureData& GetPostureData() const noexcept;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSwitchPosture OnSwitchPosture;

private:
	UPROPERTY(Transient)
	class APTCharacter* Owner;

	UPROPERTY(Transient)
	class UWeaponComponent* WeaponComp;

	UPROPERTY(Transient)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(Transient)
	class UCharacterMovementComponent* MovementComp;

	const FPostureData* PostureData;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EPostureState State = EPostureState::Stand;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bPostureSwitching : 1;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsSprinting : 1;
};
