// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WireComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTT_API UWireComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	UWireComponent();

	void Initialize(uint32 Key);

	UFUNCTION(BlueprintCallable)
	void Hook();

	UFUNCTION(BlueprintCallable)
	void Unhook();

	UFUNCTION(BlueprintCallable)
	void MoveTo();

private:
	void BeginPlay() override;

	void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction);

	void GetLifetimeReplicatedProps
		(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHook();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnhook();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMove();

	void ServerHook_Implementation();
	void ServerUnhook_Implementation();
	void ServerMove_Implementation();

	FORCEINLINE bool ServerHook_Validate() const noexcept { return true; }
	FORCEINLINE bool ServerUnhook_Validate() const noexcept { return true; }
	FORCEINLINE bool ServerMove_Validate() const noexcept { return true; }

	void OnLoadDataTable(const TSoftObjectPtr<class UDataTable>& DataTable);
	void SetCableMaterial();

	void Swing();
	void Move();

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UDataTable> WireDataTable;

	UPROPERTY(Transient)
	TSoftObjectPtr<class UMaterialInterface> MaterialCache;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class ACableActor> CableClass;

	UPROPERTY(Replicated)
	ACableActor* Cable;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FName CollisionProfile;

	FName HandSocket;

	FVector Point;
	FVector Normal;

	float Distance;

	uint32 Key;

	uint8 bMoved : 1;
	uint8 bHooked : 1;
	uint8 bCanHook : 1;
	uint8 bUseWire : 1;
	uint8 bLoadingAsset : 1;
};
