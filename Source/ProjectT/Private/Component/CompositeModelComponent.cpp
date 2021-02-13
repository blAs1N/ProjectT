// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CompositeModelComponent.h"
#include "SkeletalMeshMerge.h"
#include "Library/AsyncLoad.h"

void UCompositeModelComponent::SetParam(const FCompositeModelParam& Param)
{
	if (GetOwner() == nullptr)
		return;
	
	auto Pieces = Param.Pieces;
	Pieces.RemoveAll([Skeleton = Param.Skeleton](const auto& Ptr)
		{
			if (!Ptr) return true;
			if (Ptr->Skeleton != Skeleton)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s's skeleton is not correct"), *Ptr->GetName());
				return true;
			}
			return false;
		});

	if (Param.Pieces.Num() > 0)
	{
		auto TargetMesh = NewObject<USkeletalMesh>(this);
		TargetMesh->Skeleton = Param.Skeleton;
		
		TArray<FSkelMeshMergeSectionMapping> sectionMappings;
		FSkeletalMeshMerge merger{ TargetMesh, Pieces, sectionMappings, 0 };

		check(merger.DoMerge());
		SetSkeletalMesh(TargetMesh);
	}
	else SetSkeletalMesh(nullptr);
}
