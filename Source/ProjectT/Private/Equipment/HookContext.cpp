// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/HookContext.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CableComponent.h"
#include "Data/Hook/HookContextParam.h"
#include "Equipment/Hook.h"

void UHookContext::Initialize(const FHookContextParam& Param)
{
	Hook = Param.Hook;
	Target = Hook->GetOwner<ACharacter>();

	CollisionProfile = Param.CollisionProfile;
	HandSocket = Param.HandSocket;
	
	Stat = Param.Stat;
	HookTolerance = Param.HookTolerance;
	MoveTolerance = Param.MoveTolerance;
}

void UHookContext::TraceHookTarget()
{
	FVector Start;
	Target->GetActorEyesViewPoint(Start, HookRot);
	HookLoc = Start + (HookRot.Vector() * Stat.Distance);

	FHitResult Result;
	const bool bHit = Hook->GetWorld()->
		LineTraceSingleByProfile(Result, Start, HookLoc, CollisionProfile);

	if (bHit)
	{
		HookTarget = Result.Component.Get();
		FirstTargetLoc = HookTarget->GetComponentLocation();

		HookLoc = Result.Location;
		HookRot = FRotationMatrix::MakeFromX(Result.Normal).Rotator();
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

void UHookContext::SetVisibility(bool bNewVisibility)
{
	Hook->GetHookMesh()->SetVisibility(bNewVisibility);
	Hook->GetCable()->SetVisibility(bNewVisibility);
}

FVector UHookContext::GetHookLocation() const
{
	if (!HookTarget)
		return HookLoc;

	return HookLoc + HookTarget->
		GetComponentLocation() - FirstTargetLoc;
}

FVector UHookContext::GetHandLocation() const
{
	return Target->GetMesh()->GetSocketLocation(HandSocket);
}

void UHookContext::MulticastSetMovementMode_Implementation(
	EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	if (NewMovementMode != EMovementMode::MOVE_None || NewCustomMode != 0)
		Target->GetCharacterMovement()->SetMovementMode(NewMovementMode, NewCustomMode);
	else
		Target->GetCharacterMovement()->SetDefaultMovementMode();
}

void UHookContext::MulticastSetCollision_Implementation(bool bEnableCollision)
{
	Target->SetActorEnableCollision(bEnableCollision);
}

void UHookContext::MulticastSetLength_Implementation(float NewLength)
{
	Hook->GetCable()->CableLength = Length = NewLength;
}
