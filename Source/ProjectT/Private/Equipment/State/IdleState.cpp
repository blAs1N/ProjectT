// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/State/IdleState.h"
#include "Equipment/Hook.h"
#include "Equipment/HookContext.h"

void FIdleState::Enter(UHookContext* Context)
{
	Context->GetHook()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Context->SetVisibility(false);
	Context->SetLength(0.0f);
}
