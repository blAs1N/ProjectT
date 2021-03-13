// Fill out your copyright notice in the Description page of Project Settings.

#include "MISC/NetworkObject.h"
#include "Engine/NetDriver.h"
#include "GameFramework/Actor.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Net/UnrealNetwork.h"

bool UNetworkObject::IsSupportedForNetworking() const
{
	return true;
}

int32 UNetworkObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	const auto Owner = GetTypedOuter<AActor>();
	return Owner ? Owner->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local;
}

bool UNetworkObject::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack)
{
	if (AActor* MyOwner = GetTypedOuter<AActor>())
	{
		if (UNetDriver* NetDriver = MyOwner->GetNetDriver())
		{
			NetDriver->ProcessRemoteFunction(MyOwner, Function, Parameters, OutParms, Stack, this);
			return true;
		}
	}

	return false;
}

bool UNetworkObject::ReplicateSubobjects(UActorChannel*
	Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	return false;
}
