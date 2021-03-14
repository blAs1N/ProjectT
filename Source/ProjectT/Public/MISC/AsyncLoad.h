  // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UObject/SoftObjectPtr.h"

template <class T, class Func>
static void AsyncLoad(const TSoftObjectPtr<T>& SoftPtr, Func&& Fn)
{
	if (SoftPtr.IsNull())
		return Fn(TSoftObjectPtr<T>{ nullptr });

	if (SoftPtr.IsPending())
	{
		auto Callback = FStreamableDelegate::CreateLambda([SoftPtr, Fn = Forward<Func>(Fn)] { Fn(SoftPtr); });
		UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftPtr.ToSoftObjectPath(), MoveTemp(Callback));
	}
	else Fn(SoftPtr);
}

template <class T, class Func>
static void LoadObject(const TSoftObjectPtr<T>& SoftPtr, Func&& Fn, bool bLoadAsync)
{
	if (bLoadAsync)
	{
		AsyncLoad(SoftPtr, [Fn = Forward<Func>(Fn)](auto Ptr) { Fn(Ptr); });
	}
	else
	{
		SoftPtr.LoadSynchronous();
		Fn(SoftPtr);
	}
}
