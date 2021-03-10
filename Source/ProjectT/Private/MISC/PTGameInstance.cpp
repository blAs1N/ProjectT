// Fill out your copyright notice in the Description page of Project Settings.

#include "MISC/PTGameInstance.h"
#include "MISC/MergeModelStorage.h"

void UPTGameInstance::Init()
{
	Super::Init();
	Storage = NewObject<UMergeModelStorage>();
}
