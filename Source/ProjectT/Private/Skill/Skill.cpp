// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/Skill.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"

void USkill::Initialize(const UDataAsset* Data)
{
	ReceiveInitialize(Data);
}

UWorld* USkill::GetWorld() const
{
	const auto* Outer = GetTypedOuter<AActor>();
	return Outer ? Outer->GetWorld() : nullptr;
}
