// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/HookContext.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "CableComponent.h"
#include "Data/Hook/HookContextParam.h"
#include "Equipment/Hook.h"

void UHookContext::Initialize(const FHookContextParam& Param)
{
	Hook = GetTypedOuter<AHook>();
	Target = Hook->GetOwner<ACharacter>();

	CollisionProfile = Param.CollisionProfile;
	HandSocket = Param.HandSocket;

	Stat = Param.Stat;
	HookTolerance = Param.HookTolerance;
	MoveTolerance = Param.MoveTolerance;
}

void UHookContext::TraceHookTarget()
{
	if (HookTarget)
		HookTarget->OnEndPlay.RemoveDynamic(this, &UHookContext::OnEndTarget);

	FVector Start;
	Target->GetActorEyesViewPoint(Start, HookRot);
	HookLoc = Start + (HookRot.Vector() * Stat.Distance);

	FHitResult Result;
	Hook->GetWorld()->LineTraceSingleByProfile(
		Result, Start, HookLoc, CollisionProfile);

	if (Result.Actor.IsValid())
	{
		HookTarget = Result.GetActor();
		FirstTargetLoc = HookTarget->GetActorLocation();

		HookLoc = Result.Location;
		HookRot = FRotationMatrix::MakeFromX(Result.Normal).Rotator();

		HookTarget->OnEndPlay.AddDynamic(this, &UHookContext::OnEndTarget);
	}
	else
	{
		HookTarget = nullptr;
		FirstTargetLoc = FVector::ZeroVector;
		HookRot += FRotator{ 0.0f, 180.0f, 0.0f };
	}
}

void UHookContext::SetState(EHookState NewState)
{
	Hook->SetState(NewState);
}

FVector UHookContext::GetHookLocation() const
{
	if (!HookTarget)
		return HookLoc;

	return HookLoc + HookTarget->
		GetActorLocation() - FirstTargetLoc;
}

FVector UHookContext::GetHandLocation() const
{
	return Target->GetMesh()->GetSocketLocation(HandSocket);
}

void UHookContext::MulticastSetLength_Implementation(float NewLength)
{
	Hook->GetCable()->CableLength = Length = NewLength;
}

void UHookContext::MulticastSetAirCtrl_Implementation(float NewAirCtrl)
{
	Target->GetCharacterMovement()->AirControl = NewAirCtrl;
}

void UHookContext::MulticastSetVisibility_Implementation(bool bNewVisibility)
{
	Hook->GetHookMesh()->SetVisibility(bNewVisibility);
	Hook->GetCable()->SetVisibility(bNewVisibility);
}

void UHookContext::OnEndTarget(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	HookTarget = nullptr;
	HookLoc = FVector::ZeroVector;
	HookRot = FRotator::ZeroRotator;

	Hook->Unhook();
}
