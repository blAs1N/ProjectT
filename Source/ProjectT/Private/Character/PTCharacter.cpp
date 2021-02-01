// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PTCharacter.h"
#include "Component/CompositeModelComponent.h"

APTCharacter::APTCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCompositeModelComponent>(MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
}
