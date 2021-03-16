// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/State/StateBase.h"

class PROJECTT_API FSwingState final : public FStateBase
{
public:
	void Enter(UHookContext* Context) override;
	void Tick(UHookContext* Context, float DeltaSeconds) override;
	void Exit(UHookContext* Context) override;

private:
	FVector BeforeLoc;
	float DefaultAirCtrl;
};
