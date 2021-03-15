// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill.generated.h"

UCLASS()
class PROJECTT_API USkill : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(const class UDataAsset* Data);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Use(class UWeaponContext* Context);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Tick(float DeltaSeconds, UWeaponContext* Context);

	UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Initialize"))
	void ReceiveInitialize(const UDataAsset* Data);

	virtual void Use_Implementation(UWeaponContext* Context) {}
	virtual void Tick_Implementation(float DeltaSeconds, UWeaponContext* Context) {}
};
