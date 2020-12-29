// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Data/WeaponData.h"
#include "Library/PTStatics.h"

UWeaponComponent::UWeaponComponent()
{
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::Initialize(const FWeaponData& WeaponData)
{
	Stat = WeaponData.Stat;
	Spread = Stat.MinSpread;

	FPTStatics::AsyncLoad(WeaponData.Mesh, [this, WeaponData]
	{
		SetSkeletalMesh(WeaponData.Mesh.Get());
	});

	SetRelativeTransform(WeaponData.Transform);
	AttachToComponent(GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform, WeaponData.Socket);
}

void UWeaponComponent::StartFire()
{
	ServerStartFire();
}

void UWeaponComponent::StopFire()
{
	ServerStopFire();
}

void UWeaponComponent::TickComponent(float DeltaTime,
	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bFiring)
	{
		const auto Delay = 1.0f / (bZooming ? Stat.ZoomSpeed : Stat.Speed);
		for (; FireLag >= Delay; FireLag -= Delay)
			Shoot();

		FireLag += DeltaTime;
		return;
	}
	
	Spread = FMath::Max(Spread - (Stat.SpreadDec * DeltaTime), bZooming ? Stat.ZoomMinSpread : Stat.MinSpread);
}

void UWeaponComponent::ServerStartFire_Implementation() noexcept
{
	FireLag = 1.0f / (bZooming ? Stat.ZoomSpeed : Stat.Speed);
	bFiring = true;
}

void UWeaponComponent::Shoot()
{
	UE_LOG(LogTemp, Log, TEXT("Shoot!"));
	Spread = FMath::Max(Spread + Stat.SpreadInc, bZooming ? Stat.ZoomMaxSpread : Stat.MaxSpread);
}