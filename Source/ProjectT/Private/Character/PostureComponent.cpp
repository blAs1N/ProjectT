// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PostureComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Data/PostureData.h"

UPostureComponent::UPostureComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPostureComponent::Initialize(const FPostureData* InPostureData)
{
	CharacterOwner = Cast<ACharacter>(GetOwner());
	CapsuleComp = CharacterOwner->GetCapsuleComponent();
	MovementComp = CharacterOwner->GetCharacterMovement();

	PostureData = InPostureData;
	State = EPostureState::Crouch; // This is the trick to running "SetPosture". Actually, it is initialized to the Stand state.
	SetPosture(EPostureState::Stand);
}

void UPostureComponent::SetPosture(EPostureState NewState)
{
	if (bSwitchDelaying || State == NewState || MovementComp->IsFalling()) return;

	const bool bSwitchProne = State == EPostureState::Prone || NewState == EPostureState::Prone;

	SetPostureImpl(NewState);
	ServerSetPosture(NewState);

	if (const UWorld* World = GetWorld())
	{
		const float Delay = bSwitchProne ? ProneSwitchDelay : PostureSwitchDelay;
		WalkSpeed = MovementComp->MaxWalkSpeed;
		MovementComp->MaxWalkSpeed = 0.0f;
		bSwitchDelaying = true;

		World->GetTimerManager().SetTimer(DelayTimer, [this]
		{
			MovementComp->MaxWalkSpeed = WalkSpeed;
			bSwitchDelaying = false;
		}, Delay, false);
	}
}

void UPostureComponent::SetSprint(bool bIsSprint)
{
	if (bIsSprinting == bIsSprint) return;

	SetSprintImpl(bIsSprint);
	ServerSetSprint(bIsSprint);
}

void UPostureComponent::MulticastSetPosture_Implementation(EPostureState NewState)
{
	if (!CharacterOwner->IsLocallyControlled())
		SetPostureImpl(NewState);
}

void UPostureComponent::MulticastSetSprint_Implementation(bool bIsSprint)
{
	if (!CharacterOwner->IsLocallyControlled())
		SetSprintImpl(bIsSprint);
}

void UPostureComponent::SetPostureImpl(EPostureState NewState)
{
	State = NewState;
	if (State != EPostureState::Stand)
		SetSprintImpl(false);

	const FPostureData& Data = GetPostureData();
	const FPostureStat Stat = (&Data.StandData)[static_cast<uint8>(State)];
	const float Offset = Data.StandData.HalfHeight - Stat.HalfHeight + Stat.MeshOffset;
	const float ScaleZ = CharacterOwner->GetActorScale().Z;

	CharacterOwner->OnStartCrouch(Offset, Offset * ScaleZ);
	CharacterOwner->GetMesh()->UpdateComponentToWorld();

	if (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		CharacterOwner->AddActorWorldOffset({ 0.f, 0.0f, (Stat.HalfHeight - CapsuleComp->GetUnscaledCapsuleHalfHeight()) * ScaleZ });
	}
	else if (auto ClientData = CharacterOwner->GetCharacterMovement()->GetPredictionData_Client_Character())
	{
		ClientData->MeshTranslationOffset = FVector::ZeroVector;
		ClientData->OriginalMeshTranslationOffset = FVector::ZeroVector;
	}

	CapsuleComp->SetCapsuleHalfHeight(Stat.HalfHeight);
	CapsuleComp->SetCapsuleRadius(Stat.Radius);
	MovementComp->MaxWalkSpeed = Data.DefaultWalkSpeed * Stat.SpeedRatio;
}

void UPostureComponent::SetSprintImpl(bool bIsSprint)
{
	if (bIsSprint && State != EPostureState::Stand)
		SetPostureImpl(EPostureState::Stand);
	
	const FPostureData& Data = GetPostureData();
	const float Ratio = bIsSprint ? Data.SprintSpeedRatio : Data.StandData.SpeedRatio;
	MovementComp->MaxWalkSpeed = Data.DefaultWalkSpeed * Ratio;
	bIsSprinting = bIsSprint;
}

const FPostureData& UPostureComponent::GetPostureData() const noexcept
{
	const static FPostureData DefaultData;
	return PostureData ? *PostureData : DefaultData;
}
