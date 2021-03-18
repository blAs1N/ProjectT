// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CompositeModelComponent.h"
#include "SkeletalMeshMerge.h"
#include "Game/PTGameInstance.h"
#include "Interface/Initializable.h"
#include "MISC/AsyncLoad.h"
#include "MISC/MergeModelStorage.h"

void UCompositeModelComponent::SetParam(const FCompositeModelParam& InParam)
{
	if (InParam.Skeleton.IsNull())
	{
		SetSkeletalMesh(nullptr);
		return;
	}

	Param = InParam;
	Param.Pieces.RemoveAll([](auto Piece) {	return Piece.IsNull(); });

	PieceNum = Param.Pieces.Num();
	if (PieceNum > 0)
	{
		const bool bLoadAsync = IInitializable::Execute_IsLoadAsync(GetOwner());
		if (bLoadAsync)
			AsyncLoad(Param.Skeleton, [this](auto Skeleton) { OnLoadSkeleton(Skeleton); });
		else
			LoadSync();
	}
	else SetSkeletalMesh(nullptr);
}

void UCompositeModelComponent::SetSkeletalMesh(USkeletalMesh* NewMesh, bool bReinitPose)
{
	Super::SetSkeletalMesh(NewMesh, bReinitPose);
	OnSetMesh.Broadcast(NewMesh);
}

void UCompositeModelComponent::LoadSync()
{
	Skeleton = Param.Skeleton.LoadSynchronous();
	for (const auto& Piece : Param.Pieces)
	{
		const auto RawPiece = Piece.LoadSynchronous();
		if (RawPiece->Skeleton == Skeleton)
			Pieces.Add(RawPiece);
	}

	if (Pieces.Num() <= 0)
	{
		SetSkeletalMesh(nullptr);
		return;
	}

	Merge();
}

void UCompositeModelComponent::OnLoadSkeleton(const TSoftObjectPtr<USkeleton>& InSkeleton)
{
	check(InSkeleton.IsValid());

	Skeleton = InSkeleton.Get();
	for (auto Piece : Param.Pieces)
		AsyncLoad(Piece, [this](auto Piece) { OnLoadPiece(Piece); });
}

void UCompositeModelComponent::OnLoadPiece(const TSoftObjectPtr<USkeletalMesh>& InPiece)
{
	check(InPiece.IsValid());

	const auto Piece = InPiece.Get();
	if (Piece->Skeleton == Skeleton)
		Pieces.Add(Piece);
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
	const auto* GI = GetWorld()->GetGameInstance<UPTGameInstance>();
	const auto Model = GI ? GI->GetStorage()->
		GetMergedModel(Skeleton, Pieces) : MergeDirect();

	SetSkeletalMesh(Model);

	const bool bLoadAsync = IInitializable::Execute_IsLoadAsync(GetOwner());
	LoadObject(Param.AnimClass, [this](const auto& Ptr)
		{
			SetAnimClass(Ptr.Get());
		}, bLoadAsync);

	Pieces.Empty();
	Skeleton = nullptr;
}

USkeletalMesh* UCompositeModelComponent::MergeDirect()
{
	const auto Model = NewObject<USkeletalMesh>(this);
	Model->Skeleton = Skeleton;

	TArray<FSkelMeshMergeSectionMapping> sectionMappings;
	FSkeletalMeshMerge merger{ Model, Pieces, sectionMappings, 0 };

	check(merger.DoMerge());
	return Model;
}
