#pragma once

#include "UObject/NameTypes.h"

struct FHookContextParam final
{
	FName CollisionProfile;
	FName HandSocket;

	const struct FHookStat& Stat;
	float HookTolerance;
	float MoveTolerance;
};
