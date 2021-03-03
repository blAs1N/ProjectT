// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Hook.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CableComponent.h"
#include "Data/HookData.h"

AHook::AHook()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Hook = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hook"));
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));

	MovementComp->bAutoActivate = false;
	Cable->SetAttachEndToComponent(Hook);
}

void AHook::Initialize(const FHookData& Data)
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	check(Owner);

	Cable->AttachToComponent(Owner->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, Data.HandSocket);
}

void AHook::BeginPlay()
{
	Super::BeginPlay();

	Hook->SetVisibility(false);
	Cable->SetVisibility(false);
}

void AHook::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AHook::GetLifetimeReplicatedProps
	(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AHook::ServerHook_Implementation()
{
	if (bMoving || bThrowing || bHooked) return;

	// Check by line trace

	StartPoint = GetActorLocation();
	bThrowing = true;
}

void AHook::ServerUnhook_Implementation()
{
	if (bMoving || bThrowing || !bHooked) return;
}

void AHook::ServerMoveTo_Implementation()
{
	if (bMoving || bThrowing || !bHooked) return;

	bMoving = true;
}

void AHook::SetHookState(bool bIsHooked)
{
	bHooked = bIsHooked;
	Hook->SetVisibility(bHooked);
	Cable->SetVisibility(bHooked);
}