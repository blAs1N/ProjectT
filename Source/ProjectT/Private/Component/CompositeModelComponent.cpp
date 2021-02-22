// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CompositeModelComponent.h"
#include "SkeletalMeshMerge.h"
#include "Library/AsyncLoad.h"

void UCompositeModelComponent::SetParam(const FCompositeModelParam& InParam)
{
	if (PieceNum > 0 || InParam.Skeleton.IsNull()) return;

	Param = InParam;
	Param.Pieces.RemoveAll([](auto Piece) {	return Piece.IsNull(); });

	PieceNum = Param.Pieces.Num();
	if (PieceNum == 0u) return;

	AsyncLoad(Param.Skeleton, [this](auto Skeleton) { OnLoadSkeleton(Skeleton); });
}

void UCompositeModelComponent::OnLoadSkeleton(const TSoftObjectPtr<USkeleton>& Skeleton)
{
	check(Skeleton.IsValid());

	TargetMesh = NewObject<USkeletalMesh>(this);
	TargetMesh->Skeleton = Skeleton.Get();

	for (auto Piece : Param.Pieces)
		AsyncLoad(Piece, [this](auto Piece) { OnLoadPiece(Piece); });
}

void UCompositeModelComponent::OnLoadPiece(const TSoftObjectPtr<USkeletalMesh>& Piece)
{
	check(Piece.IsValid());

	Pieces.Add(Piece.Get());
	if (Pieces.Num() < PieceNum)
		return;

	TArray<FSkelMeshMergeSectionMapping> sectionMappings;
	FSkeletalMeshMerge merger{ TargetMesh, Pieces, sectionMappings, 0 };

	check(merger.DoMerge());
	SetSkeletalMesh(TargetMesh);
	SetAnimClass(Param.AnimClass);

	TargetMesh = nullptr;
	Pieces.Empty();
}
