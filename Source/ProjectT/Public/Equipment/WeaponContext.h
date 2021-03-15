// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MISC/NetworkObject.h"
#include "WeaponContext.generated.h"

UCLASS(BlueprintType)
class PROJECTT_API UWeaponContext final : public UNetworkObject
{
	GENERATED_BODY()
	
public:
	void Initialize(const struct FWeaponContextParam& Param);
};
