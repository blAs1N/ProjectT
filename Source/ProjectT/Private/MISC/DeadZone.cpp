// Fill out your copyright notice in the Description page of Project Settings.

#include "MISC/DeadZone.h"
#include "Components/StaticMeshComponent.h"

ADeadZone::ADeadZone()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh->SetGenerateOverlapEvents(false);
}

FBox ADeadZone::GetDeadBox() const
{
	const FVector MyLoc = GetActorLocation();
	const FVector MySize = GetActorScale3D() * 50.0f;

	return FBox::BuildAABB(MyLoc, MySize);
}

FBox ADeadZone::GetSafeDeadBox() const
{
	const FVector MyLoc = GetActorLocation();
	const FVector MySize =
		(GetActorScale3D() - ZoneSafeVector) * 50.0f;

	return FBox::BuildAABB(MyLoc, MySize);
}

void ADeadZone::BeginPlay()
{
	Super::BeginPlay();

	Loc = GetActorLocation();
	Size = GetActorScale3D();
}

void ADeadZone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeElapsed += DeltaSeconds;
	const FVector NewSize = Curve->GetFloatValue(TimeElapsed) * Size;
	
	SetActorScale3D(NewSize);

	FVector NewLoc{ Loc };
	NewLoc.Z -= 50.0f * (Size.Z - NewSize.Z);

	SetActorLocation(NewLoc);
}
