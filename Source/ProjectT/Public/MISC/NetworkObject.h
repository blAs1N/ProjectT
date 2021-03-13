// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkObject.generated.h"

UCLASS(Abstract, Blueprintable)
class PROJECTT_API UNetworkObject : public UObject
{
	GENERATED_BODY()
	
public:
	bool IsSupportedForNetworking() const override;
	int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack) override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel,
		class FOutBunch* Bunch, FReplicationFlags* RepFlags);
};
