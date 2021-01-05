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
	void Initialize(const struct FPostureData* InPostureData);

	UFUNCTION(BlueprintCallable)
	void ChangePosture(EPostureState NewState);

	UFUNCTION(BlueprintCallable)
	void Sprint();

	UFUNCTION(BlueprintCallable)
	void Unsprint();

	FORCEINLINE EPostureState GetPostureState() const noexcept { return State; }
	FORCEINLINE bool IsSprinting() const noexcept { return bIsSprinting; }

private:
	const FPostureData& GetPostureData() const noexcept;

private:
	UPROPERTY(Transient)
	class ACharacter* CharacterOwner;

	UPROPERTY(Transient)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(Transient)
	class UCharacterMovementComponent* MovementComp;

	const FPostureData* PostureData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EPostureState State = EPostureState::Stand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsSprinting : 1;
};
