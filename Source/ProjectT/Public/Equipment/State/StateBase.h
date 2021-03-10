// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ObjectMacros.h"

class PROJECTT_API FStateBase
{
public:
	virtual ~FStateBase() = default;

	virtual void Enter() {}
	virtual void Tick(float DeltaSeconds) {}
	virtual void Exit() {}

	FORCEINLINE void SetOwner(class AActor* NewOwner) noexcept { Owner = NewOwner; }

protected:
	template <class T = AActor>
	FORCEINLINE T* GetOwner() const noexcept { return Cast<T>(Owner); }

private:
	AActor* Owner;
};
