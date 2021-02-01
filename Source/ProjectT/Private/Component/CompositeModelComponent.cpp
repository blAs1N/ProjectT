// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CompositeModelComponent.h"
#include "SkeletalMeshMerge.h"

UCompositeModelComponent::UCompositeModelComponent()
{
	bWantsInitializeComponent = true;
}

void UCompositeModelComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Initialize();
}

#if WITH_EDITOR

void UCompositeModelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!PropertyChangedEvent.Property) return;

	static const FName& SkeletonName = GET_MEMBER_NAME_CHECKED(UCompositeModelComponent, Skeleton);
	static const FName& PiecesName = GET_MEMBER_NAME_CHECKED(UCompositeModelComponent, Pieces);

	if (PropertyChangedEvent.GetPropertyName() == SkeletonName)
	{
		Pieces.Empty();
		Initialize();
	}

	if (PropertyChangedEvent.GetPropertyName() == PiecesName)
		Initialize();
}

#endif

void UCompositeModelComponent::Initialize()
{
	if (GetOwner() == nullptr)
		return;
	
	const int32 PieceNum = Pieces.Num();
	TArray<USkeletalMesh*> mergeMeshes;
	for (int32 i = 0; i < PieceNum; ++i)
	{
		if (!Pieces[i]) continue;

		if (Pieces[i]->Skeleton != Skeleton)
		{
			UE_LOG(LogTemp, Warning, TEXT("The skeletons of the pieces must match"));
			Pieces[i] = nullptr;
			continue;
		}
	
		mergeMeshes.Add(Pieces[i]);
	}

	if (mergeMeshes.Num() > 0)
	{
		USkeletalMesh* targetMesh{ NewObject<USkeletalMesh>(this) };
		targetMesh->Skeleton = Skeleton;

		TArray<FSkelMeshMergeSectionMapping> sectionMappings;
		FSkeletalMeshMerge merger{ targetMesh, mergeMeshes, sectionMappings, 0 };

		check(merger.DoMerge());
		SetSkeletalMesh(targetMesh);
	}
	else SetSkeletalMesh(nullptr);
}
