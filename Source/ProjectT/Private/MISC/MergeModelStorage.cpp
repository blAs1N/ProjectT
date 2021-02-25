// Fill out your copyright notice in the Description page of Project Settings.

#include "MISC/MergeModelStorage.h"
#include "Engine/SkeletalMesh.h"
#include "SkeletalMeshMerge.h"

USkeletalMesh* UMergeModelStorage::GetMergedModel(USkeleton* Skeleton, const TArray<USkeletalMesh*>& Pieces)
{
	uint32 Hash = GetTypeHash(Skeleton);
	for (const auto* Piece : Pieces)
		Hash = HashCombine(Hash, GetTypeHash(Piece));

	if (Storage.Contains(Hash))
		return Storage[Hash];

	const auto Model = NewObject<USkeletalMesh>(this);
	Model->Skeleton = Skeleton;

	TArray<FSkelMeshMergeSectionMapping> sectionMappings;
	FSkeletalMeshMerge merger{ Model, Pieces, sectionMappings, 0 };

	check(merger.DoMerge());
	Storage.Add(Hash, Model);
	return Model;
}
