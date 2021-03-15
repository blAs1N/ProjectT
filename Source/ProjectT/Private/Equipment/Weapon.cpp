// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Weapon.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Data/Weapon/WeaponContextParam.h"
#include "Data/Weapon/WeaponData.h"
#include "Equipment/WeaponContext.h"
#include "Interface/Reloadable.h"
#include "MISC/AsyncLoad.h"
#include "Skill/Skill.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AWeapon::Initialize(const FWeaponData& Data, bool bLoadAsync)
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy) return;

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

	if (Context) OnRep_Context();
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

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;
	
	Context = NewObject<UWeaponContext>(this);
	OnRep_Context();
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	OnRep_Owner();

	if (!HasAuthority()) return;

	Context = NewObject<UWeaponContext>(this);
	OnRep_Context();
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

	for (const auto Skill : Skills)
		Skill->Tick(DeltaSeconds, Context);
}

void AWeapon::GetLifetimeReplicatedProps(TArray
	<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, Context);
}

bool AWeapon::ReplicateSubobjects(UActorChannel* Channel,
	FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (Context)
		WroteSomething |= Channel->ReplicateSubobject(Context, *Bunch, *RepFlags);

	return WroteSomething;
}

void AWeapon::OnRep_Context()
{
	if (!bInit || bInitContext) return;
	bInitContext = true;

	FWeaponContextParam Param{};

	Context->Initialize(Param);
}
