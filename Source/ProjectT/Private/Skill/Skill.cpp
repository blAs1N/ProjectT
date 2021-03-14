// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/Skill.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"

void USkill::Initialize(const UDataAsset* Data)
{
	ReceiveInitialize(Data);
}

void USkill::Begin()
{
	if (!CanUseSkill()) return;

	OnBegin();
	ReceiveOnBegin();
	bActive = true;
}

void USkill::Tick(float DeltaSeconds)
{
	if (!bActive) return;

	OnTick(DeltaSeconds);
	ReceiveOnTick(DeltaSeconds);
}

void USkill::End()
{
	if (!bActive) return;

	OnEnd();
	ReceiveOnEnd();
	bActive = false;
}

UWorld* USkill::GetWorld() const
{
	const auto* Outer = GetTypedOuter<AActor>();
	return Outer ? Outer->GetWorld() : nullptr;
}
