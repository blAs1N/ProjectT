// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PostureComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Character/PTCharacter.h"
#include "Character/WeaponComponent.h"
#include "Data/PostureData.h"

UPostureComponent::UPostureComponent()
{
	SetIsReplicatedByDefault(true);
	
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UPostureComponent::Initialize(const FPostureData* InPostureData)
{
	Owner = Cast<APTCharacter>(GetOwner());
	WeaponComp = Owner->GetWeaponComp();
	CapsuleComp = Owner->GetCapsuleComponent();
	MovementComp = Owner->GetCharacterMovement();

	PostureData = InPostureData;
	SetPostureData(EPostureState::Stand);
}

void UPostureComponent::SetPosture(EPostureState NewState)
{
	check(Owner->IsLocallyControlled());
	if (bPostureSwitching || State == NewState || MovementComp->IsFalling()) return;

	SetPostureImpl(NewState);
	ServerSetPosture(NewState);
}

void UPostureComponent::SetSprint(bool bIsSprint)
{
	check(Owner->IsLocallyControlled());
	if (bSprintMode == bIsSprint) return;
	
	if (bIsSprint && (Owner->GetCharacterMovement()->IsFalling() || WeaponComp->IsFiring()))
		return;

	if (bIsSprint && WeaponComp->IsAiming())
		WeaponComp->StopAim();

	SetSprintImpl(bIsSprint);
	ServerSetSprint(bIsSprint);
}

void UPostureComponent::TickComponent(float DeltaTime,
	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Owner) return;

	if (State != EPostureState::Stand) return;

	const FVector CurAcc = MovementComp->GetCurrentAcceleration();
	FRotator DeltaRot = CurAcc.Rotation() - Owner->GetActorRotation();
	DeltaRot.Normalize();

	bIsSprinting = bSprintMode && FMath::Abs(DeltaRot.Yaw) <= 50.0f
		&& CurAcc.Size() / MovementComp->GetMaxAcceleration();

	const FPostureData& Data = GetPostureData();
	const float Ratio = bIsSprinting ? Data.SprintSpeedRatio : Data.StandData.SpeedRatio;
	MovementComp->MaxWalkSpeed = Data.DefaultWalkSpeed * Ratio;
}

void UPostureComponent::MulticastSetPosture_Implementation(EPostureState NewState)
{
	if (!Owner->IsLocallyControlled())
		SetPostureImpl(NewState);
}

void UPostureComponent::MulticastSetSprint_Implementation(bool bIsSprint)
{
	if (!Owner->IsLocallyControlled())
		SetSprintImpl(bIsSprint);
}

void UPostureComponent::SetPostureImpl(EPostureState NewState)
{
	if (NewState != EPostureState::Stand && bSprintMode)
		SetSprintImpl(false);

	const EPostureState PrevState = State;
	SetPostureData(NewState);

	if (const UWorld* World = GetWorld())
	{
		const uint8 From = static_cast<uint8>(PrevState);
		uint8 To = static_cast<uint8>(State);
		if (From < To) --To;

		const float Delay = Owner->PlayAnimMontage
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

	bSprintMode = bIsSprint;
}

void UPostureComponent::SetPostureData(EPostureState NewState)
{	
	State = NewState;
	const FPostureData& Data = GetPostureData();
	const FPostureStat Stat = (&Data.StandData)[static_cast<uint8>(State)];
	const float Offset = Data.StandData.HalfHeight - Stat.HalfHeight + Stat.MeshOffset;
	const float ScaleZ = Owner->GetActorScale().Z;

	Owner->OnStartCrouch(Offset, Offset * ScaleZ);
	Owner->GetMesh()->UpdateComponentToWorld();

	if (Owner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		Owner->AddActorWorldOffset({ 0.f, 0.0f, (Stat.HalfHeight - CapsuleComp->GetUnscaledCapsuleHalfHeight()) * ScaleZ });
	}
	else if (auto ClientData = Owner->GetCharacterMovement()->GetPredictionData_Client_Character())
	{
		ClientData->MeshTranslationOffset = FVector::ZeroVector;
		ClientData->OriginalMeshTranslationOffset = FVector::ZeroVector;
	}

	MovementComp->MaxWalkSpeed = Data.DefaultWalkSpeed * Stat.SpeedRatio;
	CapsuleComp->SetCapsuleSize(Stat.Radius, Stat.HalfHeight);
	OnSwitchPosture.Broadcast(State);
}

const FPostureData& UPostureComponent::GetPostureData() const noexcept
{
	const static FPostureData DefaultData;
	return PostureData ? *PostureData : DefaultData;
}
