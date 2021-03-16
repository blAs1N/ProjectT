// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/SwingState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/Hook/HookStat.h"
#include "Equipment/Hook.h"
#include "Equipment/HookContext.h"

void FSwingState::Enter(UHookContext* Context)
{
	const FVector HookLoc = Context->GetHookLocation();
	const FRotator HookRot = Context->GetHookRotation();

	const float Offset = Context->GetStat().PenetrationOffset;
	const auto Target = Context->GetHookTarget();
	
	const auto Hook = Context->GetHook();
	Hook->SetActorLocationAndRotation(HookLoc, HookRot);
	Hook->AddActorLocalRotation(FRotator{ 180.0f, 0.0f, 0.0f });
	Hook->AddActorLocalOffset(FVector{ Offset, 0.0f, 0.0f });
	Hook->AttachToComponent(Target, FAttachmentTransformRules::KeepWorldTransform);

	const float Length = FVector::Distance(Context->GetHandLocation(), HookLoc);
	Context->SetLength(Length);

	DefaultAirCtrl = Context->GetTarget()->GetCharacterMovement()->AirControl;
	Context->SetAirCtrl(1.0f);
}

void FSwingState::Tick(UHookContext* Context, float DeltaSeconds)
{
	const auto Target = Context->GetTarget();

	const FVector TargetLoc = Target->GetActorLocation();
	const FVector HookLoc = Context->GetHookLocation();
	
	FVector Force = TargetLoc - HookLoc;
	const float Dot = Target->GetVelocity() | Force;

	Force.Normalize();
	Target->GetCharacterMovement()->AddForce(Force * Dot * -2.0f * Context->GetStat().SwingScale);
}

void FSwingState::Exit(UHookContext* Context)
{
	Context->SetAirCtrl(DefaultAirCtrl);
}
