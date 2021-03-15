// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Reloadable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UReloadable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTT_API IReloadable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reload(class UWeaponContext* Context);

protected:
	virtual void Reload_Implementation(UWeaponContext* Context) {}
};
