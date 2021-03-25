// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/WeaponComponent.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "Character/PTCharacter.h"
#include "Component/CompositeModelComponent.h"
#include "Data/Weapon/WeaponData.h"
#include "Equipment/Weapon.h"
#include "MISC/DataTableLoader.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	WeaponClass = AWeapon::StaticClass();
}

void UWeaponComponent::BeginFire()
{
	check(WeaponInst);
	WeaponInst->BeginFire();

	ServerBeginFire();
}

void UWeaponComponent::EndFire()
{
	check(WeaponInst);
	WeaponInst->EndFire();

	ServerEndFire();
}

void UWeaponComponent::Reload()
{
	check(WeaponInst);
	WeaponInst->Reload();

	ServerReload();
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwner()->HasAuthority())
		return;

	FActorSpawnParameters Param;
	Param.Owner = GetOwner();

	WeaponInst = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Param);
	OnRep_WeaponAndKey();
}

void UWeaponComponent::OnInitialize(int32 InKey)
{
	Key = InKey;
	OnRep_WeaponAndKey();
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
	DOREPLIFETIME(UWeaponComponent, Key);
}

void UWeaponComponent::OnRep_WeaponAndKey()
{
	if (!WeaponInst || !Key) return;

	const bool bLoadAsync = IInitializable::Execute_IsLoadAsync(GetOwner());
	GetData<FWeaponData>(WeaponDataTable, Key, [this, bLoadAsync](const auto& Data)
		{
			OnGetData(Data);

			if (WeaponInst)
				WeaponInst->Initialize(Data, bLoadAsync);
		}, bLoadAsync);
}

void UWeaponComponent::ServerBeginFire_Implementation()
{
	if (!GetOwner()->HasLocalNetOwner())
		WeaponInst->BeginFire();
}

void UWeaponComponent::ServerEndFire_Implementation()
{
	if (!GetOwner()->HasLocalNetOwner())
		WeaponInst->EndFire();
}

void UWeaponComponent::ServerReload_Implementation()
{
	if (!GetOwner()->HasLocalNetOwner())
		WeaponInst->Reload();
}
