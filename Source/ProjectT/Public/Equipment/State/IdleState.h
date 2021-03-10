// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/State/StateBase.h"

class PROJECTT_API FIdleState final : public FStateBase
{
public:
	void Enter() override;
	void Tick(float DeltaSeconds) override {}
	void Exit() override {}
};
