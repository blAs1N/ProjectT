// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/CompositeModelParam.h"
#include "CompositeModelComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetMesh, const USkeletalMesh*, Mesh);

UCLASS()
class PROJECTT_API UCompositeModelComponent final : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintSetter)
	void SetParam(const FCompositeModelParam& InParam);

	void SetSkeletalMesh(USkeletalMesh* NewMesh, bool bReinitPose = true) override;

private:
	void LoadSync();

	void OnLoadSkeleton(const TSoftObjectPtr<USkeleton>& InSkeleton);
	void OnLoadPiece(const TSoftObjectPtr<USkeletalMesh>& InPiece);

	void Merge();
	USkeletalMesh* MergeDirect();

public:
	UPROPERTY(BlueprintAssignable)
	FOnSetMesh OnSetMesh;

private:
	UPROPERTY(Transient)
	FCompositeModelParam Param;

	UPROPERTY(Transient)
	TArray<USkeletalMesh*> Pieces;

	UPROPERTY(Transient)
	USkeleton* Skeleton;

	int32 PieceNum;
};
