// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/CompositeModelParam.h"
#include "CompositeModelComponent.generated.h"

UCLASS()
class PROJECTT_API UCompositeModelComponent final : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintSetter)
	void SetParam(const FCompositeModelParam& InParam);

private:
	void OnLoadSkeleton(const TSoftObjectPtr<USkeleton>& Skeleton);
	void OnLoadPiece(const TSoftObjectPtr<USkeletalMesh>& Piece);

private:
	UPROPERTY(Transient)
	TArray<USkeletalMesh*> Pieces;

	UPROPERTY(Transient)
	USkeletalMesh* TargetMesh;

	FCompositeModelParam Param;

	int32 PieceNum;
	uint8 bLoadingAsset : 1;
};
