// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CompositeModelComponent.h"
#include "SkeletalMeshMerge.h"
#include "Interface/Loadable.h"
#include "Library/AsyncLoad.h"

void UCompositeModelComponent::SetParam(const FCompositeModelParam& InParam)
{
	if (PieceNum > 0 || InParam.Skeleton.IsNull())
	{
		SetSkeletalMesh(nullptr);
		return;
	}

	Param = InParam;
	Param.Pieces.RemoveAll([](auto Piece) {	return Piece.IsNull(); });

	PieceNum = Param.Pieces.Num();
	if (PieceNum > 0)
	{
		const bool bLoadAsync = ILoadable::Execute_IsLoadAsync(GetOwner());
		if (bLoadAsync)
			AsyncLoad(Param.Skeleton, [this](auto Skeleton) { OnLoadSkeleton(Skeleton); });
		else
			LoadSync();
	}
	else SetSkeletalMesh(nullptr);
}

void UCompositeModelComponent::LoadSync()
{
	for (const auto& Piece : Param.Pieces)
	{
		const auto RawPiece = Piece.LoadSynchronous();
		if (RawPiece->Skeleton == Param.Skeleton)
			Pieces.Add(RawPiece);
	}

	if (Pieces.Num() <= 0)
	{
		SetSkeletalMesh(nullptr);
		return;
	}

	TargetMesh = NewObject<USkeletalMesh>(this);
	TargetMesh->Skeleton = Param.Skeleton.LoadSynchronous();
	Merge();
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

	const auto PiecePtr = Piece.Get();
	if (PiecePtr->Skeleton == Param.Skeleton)
		Pieces.Add(PiecePtr);
	else
		--PieceNum;

	if (PieceNum <= 0)
	{
		SetSkeletalMesh(nullptr);
		return;
	}

	if (Pieces.Num() >= PieceNum)
		Merge();
}

void UCompositeModelComponent::Merge()
{
	TArray<FSkelMeshMergeSectionMapping> sectionMappings;
	FSkeletalMeshMerge merger{ TargetMesh, Pieces, sectionMappings, 0 };

	check(merger.DoMerge());
	SetSkeletalMesh(TargetMesh);
	SetAnimClass(Param.AnimClass);

	//TargetMesh = nullptr;
	Pieces.Empty();
}
