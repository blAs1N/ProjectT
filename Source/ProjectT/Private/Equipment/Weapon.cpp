// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Weapon.h"
#include "Data/WeaponData.h"
#include "MISC/AsyncLoad.h"
#include "Skill/Skill.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AWeapon::Initialize(const FWeaponData& Data, bool bLoadAsync)
{
	if (!HasAuthority()) return;

	const int32 Num = Data.Skills.Num();
	Skills.Init(nullptr, Num);

	for (int32 Idx = 0; Idx < Num; ++Idx)
	{
		const auto& Skill = Data.Skills[Idx];
		
		Skills[Idx] = NewObject<USkill>(this, Skill.Class);
		LoadObject(Skill.Data, [this, Idx](const auto& Data)
			{
				Skills[Idx]->Initialize(Data.Get());
			}, bLoadAsync);
	}

	bInit = true;

	//if (Context) OnRep_Context();
}

void AWeapon::UseSkill(uint8 Index)
{
	if (Skills.IsValidIndex(Index))
		Skills[Index]->Use(Context);
}

void AWeapon::Reload()
{
	for (const auto Skill : Skills)
		IReloadable::Execute_Reload(Skill, Context);
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const int32 Num = Skills.Num();
	for (int32 Idx = 0; Idx < Num; ++Idx)
		Skills[Idx]->Tick(DeltaSeconds);
}
