// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/WireComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "CableActor.h"
#include "CableComponent.h"
#include "Data/WireData.h"
#include "Interface/Loadable.h"
#include "MISC/AsyncLoad.h"

UWireComponent::UWireComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CableClass = ACableActor::StaticClass();
}

void UWireComponent::Initialize(uint32 InKey)
{
	if (bLoadingAsset && WireDataTable.IsNull()) return;

	bLoadingAsset = true;
	Key = InKey;

	const bool bLoadAsync = ILoadable::Execute_IsLoadAsync(GetOwner());
	if (bLoadAsync)
	{
		AsyncLoad(WireDataTable, [this](auto DataTable) { OnLoadDataTable(DataTable); });
	}
	else
	{
		WireDataTable.LoadSynchronous();
		OnLoadDataTable(WireDataTable);
	}
}

void UWireComponent::Hook()
{
	ServerHook();
}

void UWireComponent::Unhook()
{
	ServerUnhook();
}

void UWireComponent::MoveTo()
{
	ServerMove();
}

void UWireComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwner()->HasAuthority()) return;

	FActorSpawnParameters Param;
	Param.Owner = Param.Instigator = Cast<APawn>(GetOwner());
	Cable = GetWorld()->SpawnActor<ACableActor>(CableClass, Param);

	if (!MaterialCache.IsNull())
		SetCableMaterial();
}

void UWireComponent::TickComponent(float DeltaTime,
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

void UWireComponent::GetLifetimeReplicatedProps
	(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWireComponent, Cable);
}

void UWireComponent::ServerHook_Implementation()
{
	if (bUseWire || bHooked || !bCanHook) return;

	const auto Owner = Cast<ACharacter>(GetOwner());
	check(Owner);

	const FVector HandLoc = Owner->GetMesh()->GetSocketLocation(HandSocket);
	Cable->CableComponent->CableLength = FVector::Distance(HandLoc, Point);
	Cable->CableComponent->SetAttachEndToComponent(Owner->GetMesh(), HandSocket);
	bHooked = true;
}

void UWireComponent::ServerUnhook_Implementation()
{
	if (!bUseWire && bHooked)
		bHooked = false;
}

void UWireComponent::ServerMove_Implementation()
{
	if (bUseWire || !bHooked)
		bMoved = true;
}

void UWireComponent::OnLoadDataTable(const TSoftObjectPtr<class UDataTable>& DataTable)
{
	static const FWireData DefaultData{};
	const auto* Data = &DefaultData;

	if (DataTable.IsValid())
	{
		const auto* TempData = DataTable.Get()->FindRow
			<FWireData>(FName{ *FString::FromInt(Key) }, TEXT(""));

		if (TempData) Data = TempData;
	}

	MaterialCache = Data->Material;
	if (Cable && !MaterialCache.IsNull()) SetCableMaterial();
	
	HandSocket = Data->HandSocket;
	Distance = Data->Distance;
	bLoadingAsset = false;
}

void UWireComponent::SetCableMaterial()
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

void UWireComponent::Swing()
{

}

void UWireComponent::Move()
{

}
