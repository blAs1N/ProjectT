// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/HookComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "CableActor.h"
#include "CableComponent.h"
#include "Data/HookData.h"
#include "Interface/Loadable.h"
#include "MISC/AsyncLoad.h"

UHookComponent::UHookComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CableClass = ACableActor::StaticClass();
}

void UHookComponent::Initialize(uint32 InKey)
{
	if (bLoadingAsset && HookDataTable.IsNull()) return;

	bLoadingAsset = true;
	Key = InKey;

	const bool bLoadAsync = ILoadable::Execute_IsLoadAsync(GetOwner());
	if (bLoadAsync)
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
	ServerMove();
}

void UHookComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwner()->HasAuthority()) return;

	FActorSpawnParameters Param;
	Param.Owner = Param.Instigator = Cast<APawn>(GetOwner());
	Cable = GetWorld()->SpawnActor<ACableActor>(CableClass, Param);

	if (!MaterialCache.IsNull())
		SetCableMaterial();
}

void UHookComponent::TickComponent(float DeltaTime,
	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Cable || !GetOwner()->HasAuthority()) return;

	const auto Owner = Cast<ACharacter>(GetOwner());
	check(Owner);

	FVector Start; FRotator Dir;
	GetOwner()->GetActorEyesViewPoint(Start, Dir);
	const FVector End = Start + (Dir.Vector() * Distance);

	FHitResult Result;
	bCanHook = GetWorld()->LineTraceSingleByProfile(Result, Start, End, CollisionProfile);
	Point = Result.ImpactPoint;
	Normal = Result.ImpactNormal;

	if (bMoved) Move();
	else if (bHooked) Swing();
	else Cable->SetActorLocation(Point);

	Cable->CableComponent->SetVisibility(bHooked, true);
}

void UHookComponent::GetLifetimeReplicatedProps
	(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHookComponent, Cable);
}

void UHookComponent::ServerHook_Implementation()
{
	if (bUseHook || bHooked || !bCanHook) return;

	const auto Owner = Cast<ACharacter>(GetOwner());
	check(Owner);

	const FVector HandLoc = Owner->GetMesh()->GetSocketLocation(HandSocket);
	Cable->CableComponent->CableLength = FVector::Distance(HandLoc, Point);
	Cable->CableComponent->SetAttachEndToComponent(Owner->GetMesh(), HandSocket);
	bHooked = true;
}

void UHookComponent::ServerUnhook_Implementation()
{
	if (!bUseHook && bHooked)
		bHooked = false;
}

void UHookComponent::ServerMove_Implementation()
{
	if (bUseHook || !bHooked)
		bMoved = true;
}

void UHookComponent::OnLoadDataTable(const TSoftObjectPtr<class UDataTable>& DataTable)
{
	static const FHookData DefaultData{};
	const auto* Data = &DefaultData;

	if (DataTable.IsValid())
	{
		const auto* TempData = DataTable.Get()->FindRow
			<FHookData>(FName{ *FString::FromInt(Key) }, TEXT(""));

		if (TempData) Data = TempData;
	}

	MaterialCache = Data->Material;
	if (Cable && !MaterialCache.IsNull()) SetCableMaterial();
	
	HandSocket = Data->HandSocket;
	Distance = Data->Distance;
	bLoadingAsset = false;
}

void UHookComponent::SetCableMaterial()
{
	const bool bLoadAsync = ILoadable::Execute_IsLoadAsync(GetOwner());
	if (bLoadAsync)
	{
		AsyncLoad(MaterialCache, [Cable = Cable](const auto& Ptr)
			{
				Cable->CableComponent->SetMaterial(0, Ptr.Get());
			});
	}
	else Cable->CableComponent->SetMaterial(0, MaterialCache.LoadSynchronous());

	MaterialCache = nullptr;
}

void UHookComponent::Swing()
{

}

void UHookComponent::Move()
{

}
