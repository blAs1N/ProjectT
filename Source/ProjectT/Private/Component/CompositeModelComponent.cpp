// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CompositeModelComponent.h"
#include "SkeletalMeshMerge.h"
#include "Library/AsyncLoad.h"

UCompositeModelComponent::UCompositeModelComponent()
{
	bWantsInitializeComponent = true;
}

void UCompositeModelComponent::SetParam(const FCompositeModelParam& InParam)
{
	if (GetOwner() == nullptr)
		return;
	
	Param.Pieces.RemoveAll([this](const auto& Ptr) { return !Ptr; });

	if (Param.Pieces.Num() > 0)
	{
		TargetMesh = NewObject<USkeletalMesh>(this);
		AsyncLoad(Param.Skeleton, [this]
			(const auto& Ptr) { OnLoadSkeleton(Ptr); });

	}
	else SetSkeletalMesh(nullptr);
}

void UCompositeModelComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetParam(Param);
}

#if WITH_EDITOR

void UCompositeModelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!PropertyChangedEvent.Property) return;

	FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if (PropertyName == TEXT("Skeleton"))
	{
		Param.Pieces.Empty();
		SetParam(Param);
	}
	else if (PropertyName == TEXT("Piece"))
		SetParam(Param);
	else if (PropertyName == TEXT("AnimClass"))
		SetAnimClass(Param.AnimClass);
}

#endif

void UCompositeModelComponent::OnLoadSkeleton(const TSoftObjectPtr<USkeleton>& Ptr)
{
	TargetMesh->Skeleton = Ptr.Get();
	LoadedPieceNum = 0;

	for (const auto& Piece : Param.Pieces)
		AsyncLoad(Piece, [this](const auto& Ptr) { OnLoadPiece(Ptr); });
}

void UCompositeModelComponent::OnLoadPiece(const TSoftObjectPtr<USkeletalMesh>& Ptr)
{
	if (Ptr->Skeleton != Param.Skeleton)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's skeleton is not correct"), *Ptr->GetName());
		Param.Pieces.Remove(Ptr);
		return;
	}

	if (++LoadedPieceNum >= Param.Pieces.Num())
	{
		const int32 PieceNum = Param.Pieces.Num();
		TArray<USkeletalMesh*> Pieces;
		Pieces.Init(nullptr, PieceNum);

		for (int32 i = 0; PieceNum; ++i)
			Pieces[i] = Param.Pieces[i].Get();

		TArray<FSkelMeshMergeSectionMapping> sectionMappings;
		FSkeletalMeshMerge merger{ TargetMesh, Pieces, sectionMappings, 0 };

		check(merger.DoMerge());
		SetSkeletalMesh(TargetMesh);
	}
}
