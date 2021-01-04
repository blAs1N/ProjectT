// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PostureComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/PostureData.h"

void UPostureComponent::Initialize(const FPostureData* InPostureData)
{
	CharacterOwner = Cast<ACharacter>(GetOwner());
	CapsuleComp = CharacterOwner->GetCapsuleComponent();
	MovementComp = CharacterOwner->GetCharacterMovement();

	PostureData = InPostureData;
	State = EPostureState::Crouch; // This is the trick to running "ChangePosture". Actually, it is initialized to the Stand state.
	ChangePosture(EPostureState::Stand);
}

void UPostureComponent::ChangePosture(EPostureState NewState)
{
	if (State == NewState) return;

	State = NewState;
	if (State != EPostureState::Stand)
		Unsprint();

	const FPostureData& Data = GetPostureData();
	const FPostureStat Stat = (&Data.StandData)[static_cast<uint8>(NewState)];
	const float ScaleZ = CharacterOwner->GetActorScale().Z;
	const float Offset = Stat.MeshOffset - CharacterOwner->GetMesh()->GetRelativeLocation().Z;

	CharacterOwner->OnStartCrouch(Offset, Offset * ScaleZ);
	CharacterOwner->GetMesh()->UpdateComponentToWorld();

	CapsuleComp->SetCapsuleHalfHeight(Stat.HalfHeight);
	CapsuleComp->SetCapsuleRadius(Stat.Radius);
	MovementComp->MaxWalkSpeed = Data.DefaultWalkSpeed * Stat.SpeedRatio;
}

void UPostureComponent::Sprint()
{
	if (bIsSprinting) return;

	ChangePosture(EPostureState::Stand);

	const FPostureData& Data = GetPostureData();
	MovementComp->MaxWalkSpeed = Data.DefaultWalkSpeed * Data.SprintSpeedRatio;
	bIsSprinting = true;
}

void UPostureComponent::Unsprint()
{
	if (!bIsSprinting) return;

	const FPostureData& Data = GetPostureData();
	MovementComp->MaxWalkSpeed = Data.DefaultWalkSpeed;
	bIsSprinting = false;
}

const FPostureData& UPostureComponent::GetPostureData() const noexcept
{
	const static FPostureData DefaultData;
	return PostureData ? *PostureData : DefaultData;
}