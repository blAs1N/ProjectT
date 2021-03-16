// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Weapon.h"
#include "Camera/CameraShake.h"
#include "Components/AudioComponent.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "Data/Weapon/WeaponData.h"
#include "MISC/AsyncLoad.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FireComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireComp"));
	FireComp->SetupAttachment(RootComponent);
	FireComp->bAutoActivate = false;
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->bAutoActivate = false;
}

void AWeapon::Initialize(const FWeaponData& Data, bool bLoadAsync)
{
	LoadObject(Data.FireSound, [AudioComp = AudioComp](const auto& Ptr)
		{
			AudioComp->SetSound(Ptr.Get());
		}, bLoadAsync);

	Stat = Data.Stat;
	Clip = Stat.Ammo;
}

void AWeapon::BeginFire()
{
	if (State == EWeaponState::Idle)
		State = EWeaponState::Fire;
}

void AWeapon::EndFire()
{
	if (State == EWeaponState::Fire)
		State = EWeaponState::Idle;
}

void AWeapon::Reload()
{
	if (State != EWeaponState::Reload)
		State = EWeaponState::Reload;
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	OnRep_Owner();
}

void AWeapon::OnRep_Owner()
{
	if (const auto MyOwner = GetOwner())
		SetRole(MyOwner->GetLocalRole());
	else
		SetRole(ROLE_None);
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FireLag += DeltaSeconds;

	if (State == EWeaponState::Fire)
	{
		for (; FireLag >= Stat.Speed; FireLag -= Stat.Speed)
			Shoot();
	}
	else
	{
		Spread = FMath::Max(Spread - Stat.SpreadDec * DeltaSeconds, Stat.MinSpread);
		FireLag = FMath::Min(FireLag, Stat.Speed);
		FireComp->Deactivate();
	}
}

void AWeapon::Shoot()
{
	const auto MyOwner = GetOwner<ACharacter>();
	
	FVector Start; FRotator Rot;
	MyOwner->GetActorEyesViewPoint(Start, Rot);

	const FVector Dir = Rot.Vector() + FVector{ FMath::RandPointInCircle(Spread), 0.0f };

	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredActor(MyOwner);

	FHitResult Result;
	GetWorld()->LineTraceSingleByChannel(Result, Start, Start +
		(Dir * Stat.Distance), ECollisionChannel::ECC_Visibility, Param);

	if (Result.Actor.IsValid())
	{
		const FVector2D InRange{ Stat.MinDamageDistance, Stat.MaxDamageDistance };
		const FVector2D OutRange{ Stat.MinDamage, Stat.MaxDamage };

		const float Damage = FMath::GetMappedRangeValueClamped(InRange, OutRange, Result.Distance);
		
		UGameplayStatics::ApplyPointDamage(Result.GetActor(), Damage, Dir,
			Result, MyOwner->GetController(), this, UDamageType::StaticClass());
	}
}
