// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/HookComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "CableActor.h"
#include "CableComponent.h"
#include "Data/HookData.h"
#include "Equipment/Hook.h"
#include "Interface/Loadable.h"
#include "MISC/AsyncLoad.h"

UHookComponent::UHookComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	HookClass = AHook::StaticClass();
}

void UHookComponent::Initialize(uint32 InKey)
{
	if (bLoadingAsset && HookDataTable.IsNull()) return;

	bLoadingAsset = true;
	Key = InKey;

	if (IsLoadAsync())
	{
		AsyncLoad(HookDataTable, [this](auto DataTable) { OnLoadDataTable(DataTable); });
	}
	else
	{
		HookDataTable.LoadSynchronous();
		OnLoadDataTable(HookDataTable);
	}
}

void UHookComponent::Hook()
{
	ServerHook();
}

void UHookComponent::Unhook()
{
	ServerUnhook();
}

void UHookComponent::MoveTo()
{
	ServerMoveTo();
}

void UHookComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwner()->HasAuthority()) return;

	FActorSpawnParameters Param;
	Param.Owner = Param.Instigator = Cast<APawn>(GetOwner());
	HookInst = GetWorld()->SpawnActor<AHook>(HookClass, Param);
	OnRep_Hook();
}

void UHookComponent::GetLifetimeReplicatedProps
	(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHookComponent, HookInst);
}

void UHookComponent::OnRep_Hook()
{
	if (Data) HookInst->Initialize(*Data, IsLoadAsync());
}

void UHookComponent::ServerHook_Implementation()
{
	check(HookInst);
	HookInst->Hook();
}

void UHookComponent::ServerUnhook_Implementation()
{
	check(HookInst);
	HookInst->Unhook();
}

void UHookComponent::ServerMoveTo_Implementation()
{
	check(HookInst);
	HookInst->MoveTo();
}

void UHookComponent::OnLoadDataTable(const TSoftObjectPtr<class UDataTable>& DataTable)
{
	static const FHookData DefaultData{};
	
	if (DataTable.IsValid())
	{
		Data = DataTable.Get()->FindRow
			<FHookData>(FName{ *FString::FromInt(Key) }, TEXT(""));
	}
	else Data = &DefaultData;

	if (HookInst)
	{
		HookInst->Initialize(*Data, IsLoadAsync());
		Data = nullptr;
	}

	bLoadingAsset = false;
}

bool UHookComponent::IsLoadAsync() const
{
	return ILoadable::Execute_IsLoadAsync(GetOwner());
}
