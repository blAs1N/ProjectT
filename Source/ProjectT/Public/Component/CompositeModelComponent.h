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
	UCompositeModelComponent();

	UFUNCTION(BlueprintSetter)
	void SetParam(const FCompositeModelParam& InParam);

private:
	void InitializeComponent() override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void OnLoadSkeleton(const TSoftObjectPtr<USkeleton>& Ptr);
	void OnLoadPiece(const TSoftObjectPtr<USkeletalMesh>& Ptr);

private:
	UPROPERTY(EditAnywhere, BlueprintSetter = SetParam, Category = Piece, meta = (AllowPrivateAccess = true))
	FCompositeModelParam Param;

	UPROPERTY(Transient)
	USkeletalMesh* TargetMesh;

	int32 LoadedPieceNum;
};
