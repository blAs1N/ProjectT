// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/Initializable.h"

void IInitializable::Initialize_Implementation(int32 Key)
{
	OnInitialize(Key);
	IInitializable::Execute_ReceiveOnInitialize(Cast<UObject>(this), Key);
}
