// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PTGameInstance.generated.h"

UCLASS()
class PROJECTT_API UPTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	class UMergeModelStorage* GetStorage() const noexcept { return Storage; }

private:
	void Init() override;

private:
	UPROPERTY(Transient)
	UMergeModelStorage* Storage;
};
