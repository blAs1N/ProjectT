// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/HookComponent.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "Data/Hook/HookData.h"
#include "Equipment/Hook.h"
#include "MISC/DataTableLoader.h"

UHookComponent::UHookComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	HookClass = AHook::StaticClass();
}

void UHookComponent::Hook()
{
	check(HookInst);
	HookInst->Hook();

	ServerHook();
}

void UHookComponent::Unhook()
{
	check(HookInst);
	HookInst->Unhook();

	ServerUnhook();
}

void UHookComponent::MoveTo()
{
	check(HookInst);
	HookInst->MoveTo();

	ServerMoveTo();
}

void UHookComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwner()->HasAuthority())
		return;
	
	FActorSpawnParameters Param;
	Param.Owner = GetOwner();

	HookInst = GetWorld()->SpawnActor<AHook>(HookClass, Param);
	OnRep_HookAndKey();
}

void UHookComponent::OnInitialize(int32 InKey)
{
	Key = InKey;
	OnRep_HookAndKey();
}

void UHookComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHookComponent, HookInst);
	DOREPLIFETIME(UHookComponent, Key);
}

void UHookComponent::OnRep_HookAndKey()
{
	if (!HookInst || !Key) return;

	const bool bLoadAsync = IInitializable::Execute_IsLoadAsync(GetOwner());
	GetData<FHookData>(HookDataTable, Key, [this, bLoadAsync](auto Data)
		{
			HookInst->Initialize(Data, bLoadAsync);
		}, bLoadAsync);
}

void UHookComponent::ServerHook_Implementation()
{
	if (!GetOwner()->HasLocalNetOwner())
		HookInst->Hook();
}

void UHookComponent::ServerUnhook_Implementation()
{
	if (!GetOwner()->HasLocalNetOwner())
		HookInst->Unhook();
}

void UHookComponent::ServerMoveTo_Implementation()
{
	if (!GetOwner()->HasLocalNetOwner())
		HookInst->MoveTo();
}
