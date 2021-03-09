// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Initializable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInitializable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTT_API IInitializable
{
	GENERATED_BODY()

public:
	// 'BlueprintNativeEvent' is a trick to write a function in an interface. So overriding is not possible.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Initialize(int32 Key);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsLoadAsync() const;

protected:
	virtual void OnInitialize(int32 Key) {}

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnInitialize"))
	void ReceiveOnInitialize(int32 Key);

	virtual bool IsLoadAsync_Implementation() const noexcept { return false; }

private:
	void Initialize_Implementation(int32 Key);
};
