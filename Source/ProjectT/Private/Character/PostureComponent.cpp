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
	SetPostureData(EPostureState::Stand);
}

void UPostureComponent::SetPosture(EPostureState NewState)
{
	if (bPostureSwitching || State == NewState || MovementComp->IsFalling()) return;

	SetPostureImpl(NewState);
	ServerSetPosture(NewState);
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
	if (NewState != EPostureState::Stand && bIsSprinting)
		SetSprintImpl(false);

	const EPostureState PrevState = State;
	SetPostureData(NewState);

	if (const UWorld* World = GetWorld())
	{
		const uint8 From = static_cast<uint8>(PrevState);
		uint8 To = static_cast<uint8>(State);
		if (From < To) --To;

		const float Delay = CharacterOwner->PlayAnimMontage
			((&PostureData->PostureSwitchAnims.StandToCrouch)[From * 2 + To]);

		const float PrevSpeed = MovementComp->MaxWalkSpeed;
		MovementComp->MaxWalkSpeed = 0.0f;
		bPostureSwitching = true;

		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [this, PrevSpeed]
			{
				MovementComp->MaxWalkSpeed = PrevSpeed;
				bPostureSwitching = false;
			}, Delay, false);
	}
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

void UPostureComponent::SetPostureData(EPostureState NewState)
{	
	State = NewState;
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

	MovementComp->MaxWalkSpeed = Data.DefaultWalkSpeed * Stat.SpeedRatio;
	CapsuleComp->SetCapsuleSize(Stat.Radius, Stat.HalfHeight);
}

const FPostureData& UPostureComponent::GetPostureData() const noexcept
{
	const static FPostureData DefaultData;
	return PostureData ? *PostureData : DefaultData;
}
