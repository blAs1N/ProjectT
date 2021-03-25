// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Initializable.h"
#include "HookComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTT_API UHookComponent final : public UActorComponent, public IInitializable
{
	GENERATED_BODY()

public:
	UHookComponent();

	UFUNCTION(BlueprintCallable)
	void Hook();

	UFUNCTION(BlueprintCallable)
	void Unhook();

	UFUNCTION(BlueprintCallable)
	void MoveTo();

private:
	void BeginPlay() override;
	void OnInitialize(int32 InKey) override;

	void GetLifetimeReplicatedProps(TArray
		<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_HookAndKey();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHook();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnhook();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveTo();

	FORCEINLINE bool ServerHook_Validate() const noexcept { return true; }
	FORCEINLINE bool ServerUnhook_Validate() const noexcept { return true; }
	FORCEINLINE bool ServerMoveTo_Validate() const noexcept { return true; }

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<class UDataTable> HookDataTable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AHook> HookClass;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_HookAndKey)
	AHook* HookInst;
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_HookAndKey)
	int32 Key;
};
