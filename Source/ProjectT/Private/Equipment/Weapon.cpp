// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Weapon.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Data/Weapon/WeaponData.h"
#include "MISC/AsyncLoad.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AWeapon::Initialize(const FWeaponData& Data, bool bLoadAsync)
{
	if (GetOwner()->GetLocalRole() >= ROLE_AutonomousProxy)
		Stat = Data.Stat;
}

void AWeapon::BeginFire()
{
	
}

void AWeapon::EndFire()
{

}

void AWeapon::Reload()
{
	
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

	
}
