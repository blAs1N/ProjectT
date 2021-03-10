// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/State/StateBase.h"

class PROJECTT_API FThrowState final : public FStateBase
{
public:
	void Enter() override;
	void Tick(float DeltaSeconds) override;
	void Exit() override {}

private:
	FVector StartLoc;
};
