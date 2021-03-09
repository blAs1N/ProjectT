// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/HookComponent.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "Data/HookData.h"
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
	Param.Owner = GetOwner();
	HookInst = GetWorld()->SpawnActor<AHook>(HookClass, Param);
	OnRep_Hook();
}

void UHookComponent::OnInitialize(int32 InKey)
{
	Key = InKey;
}

void UHookComponent::GetLifetimeReplicatedProps
	(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHookComponent, HookInst);
}

void UHookComponent::OnRep_Hook()
{
	const bool bLoadAsync = IInitializable::Execute_IsLoadAsync(GetOwner());
	GetData<FHookData>(HookDataTable, Key, [this, bLoadAsync](auto Data)
		{
			if (HookInst) HookInst->Initialize(Data, bLoadAsync);
		}, bLoadAsync);
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
