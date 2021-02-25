// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MergeModelStorage.generated.h"

UCLASS()
class PROJECTT_API UMergeModelStorage : public UObject
{
	GENERATED_BODY()
	
public:
	USkeletalMesh* GetMergedModel(class USkeleton* Skeleton, const TArray<class USkeletalMesh*>& Pieces);

private:
	UPROPERTY(Transient)
	TMap<uint32, class USkeletalMesh*> Storage;
};
