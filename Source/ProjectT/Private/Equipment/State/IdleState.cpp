// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/IdleState.h"
#include "Equipment/Hook.h"

void FIdleState::Enter()
{
	const auto Hook = GetOwner<AHook>();

	Hook->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Hook->SetVisibility(false);
	Hook->SetLength(0.0f);
}
