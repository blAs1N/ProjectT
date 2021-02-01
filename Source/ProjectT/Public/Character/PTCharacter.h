// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PTCharacter.generated.h"

UCLASS(config=Game)
class APTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APTCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
