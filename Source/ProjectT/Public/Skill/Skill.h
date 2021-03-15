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
	virtual void Initialize(const UDataAsset* Data);

	UFUNCTION(BlueprintCallable)
	void Begin();

	UFUNCTION(BlueprintCallable)
	void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void End();

	UFUNCTION(BlueprintNativeEvent)
	bool CanUseSkill() const;

	UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Initialize"))
	void ReceiveInitialize(const UDataAsset* Data);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnBegin"))
	void ReceiveOnBegin();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTick"))
	void ReceiveOnTick(float DeltaSeconds);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEnd"))
	void ReceiveOnEnd();

	virtual void OnBegin() {}
	virtual void OnTick(float DeltaSeconds) {}
	virtual void OnEnd() {}

	virtual bool CanUseSkill_Implementation() const { return !bActive; }

private:
	uint8 bActive : 1;
};
