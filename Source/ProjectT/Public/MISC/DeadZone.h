// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadZone.generated.h"

UCLASS(BlueprintType)
class PROJECTT_API ADeadZone final : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeadZone();

	FBox GetDeadBox() const;
	FBox GetSafeDeadBox() const;

	FORCEINLINE class UStaticMeshComponent* GetMesh() const noexcept { return Mesh; }

private:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	UCurveFloat* Curve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector ZoneSafeVector;

	FVector Loc;
	FVector Size;

	float TimeElapsed;
};
