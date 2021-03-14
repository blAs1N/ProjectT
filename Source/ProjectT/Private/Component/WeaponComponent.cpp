// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/WeaponComponent.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "Character/PTCharacter.h"
#include "Data/WeaponData.h"
#include "Equipment/Weapon.h"
#include "MISC/DataTableLoader.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	WeaponClass = AWeapon::StaticClass();
}

void UWeaponComponent::BeginSkill(uint8 Index)
{
	check(WeaponInst);
	WeaponInst->BeginSkill(Index);

	ServerBeginSkill(Index);
}

void UWeaponComponent::EndSkill(uint8 Index)
{
	check(WeaponInst);
	WeaponInst->EndSkill(Index);

	ServerEndSkill(Index);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwner()->HasAuthority())
		return;

	FActorSpawnParameters Param;
	Param.Owner = GetOwner();

	WeaponInst = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Param);
	OnRep_Weapon();
}

void UWeaponComponent::OnInitialize(int32 InKey)
{
	Key = InKey;
	OnRep_Weapon();
}

void UWeaponComponent::OnGetData(const FWeaponData& Data)
{
	const auto OnLoadObject = [this, Data](const auto& Ptr)
	{
		const auto* Owner = GetOwner<APTCharacter>();
		const auto WeaponMesh = Owner->GetWeaponMeshComponent();

		WeaponMesh->SetStaticMesh(Ptr.Get());
		WeaponMesh->SetRelativeTransform(Data.Transform);
		WeaponMesh->AttachToComponent(Owner->GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform, Data.HandSocket);

		if (WeaponInst)
		{
			WeaponInst->SetActorRelativeTransform(FTransform::Identity);

			WeaponInst->AttachToComponent(WeaponMesh,
				FAttachmentTransformRules::KeepRelativeTransform, Data.MuzzleSocket);
		}
	};

	const bool bLoadAsync = IInitializable::Execute_IsLoadAsync(GetOwner());
	LoadObject(Data.Mesh, OnLoadObject, bLoadAsync);
}

void UWeaponComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponComponent, WeaponInst);
}

void UWeaponComponent::OnRep_Weapon()
{
	if (!Key) return;

	const bool bLoadAsync = IInitializable::Execute_IsLoadAsync(GetOwner());
	GetData<FWeaponData>(WeaponDataTable, Key, [this, bLoadAsync](const auto& Data)
		{
			OnGetData(Data);

			if (WeaponInst)
				WeaponInst->Initialize(Data, bLoadAsync);
		}, bLoadAsync);
}

void UWeaponComponent::ServerBeginSkill_Implementation(uint8 Index)
{
	if (!GetOwner()->HasLocalNetOwner())
		WeaponInst->BeginSkill(Index);
}

void UWeaponComponent::ServerEndSkill_Implementation(uint8 Index)
{
	if (!GetOwner()->HasLocalNetOwner())
		WeaponInst->EndSkill(Index);
}
