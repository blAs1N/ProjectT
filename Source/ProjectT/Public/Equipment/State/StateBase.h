// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

class PROJECTT_API FStateBase
{
public:
	virtual void Enter(class UHookContext* Context) {}
	virtual void Tick(UHookContext* Context, float DeltaSeconds) {}
	virtual void Exit(UHookContext* Context) {}
};
