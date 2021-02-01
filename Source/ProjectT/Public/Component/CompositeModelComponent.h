// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "CompositeModelComponent.generated.h"

UCLASS()
class PROJECTT_API UCompositeModelComponent final : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UCompositeModelComponent();

private:
	void InitializeComponent() override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void Initialize();

private:
	UPROPERTY(EditAnywhere, Category = Piece, meta = (AllowPrivateAccess = true))
	USkeleton* Skeleton;

	UPROPERTY(EditAnywhere, Category = Piece, meta = (AllowPrivateAccess = true))
	TArray<USkeletalMesh*> Pieces;
};
