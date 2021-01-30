  // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UObject/SoftObjectPtr.h"

class PROJECTT_API FPTStatics
{
public:
	template <class T, class Func>
	static void AsyncLoad(const TAssetPtr<T>& SoftPtr, Func&& Fn)
	{
		if (!SoftPtr.IsNull() && SoftPtr.IsPending())
		{
			auto Callback = FStreamableDelegate::CreateLambda(Forward<Func>(Fn));
			UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftPtr.ToSoftObjectPath(), MoveTemp(Callback));
		}
		else Fn();
	}
};
