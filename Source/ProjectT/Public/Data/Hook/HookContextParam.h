#pragma once

#include "UObject/NameTypes.h"

struct FHookContextParam final
{
	class AHook* Hook;
	FName CollisionProfile;
	FName HandSocket;

	const struct FHookStat& Stat;
	float HookTolerance;
	float MoveTolerance;
};
