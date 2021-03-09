// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Initializable.h"
#include "WeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTT_API UWeaponComponent final : public UActorComponent, public IInitializable
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

private:
	void OnInitialize(int32 InKey) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<class UDataTable> WeaponDataTable;

	int32 Key;
};
