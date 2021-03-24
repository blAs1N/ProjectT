// Fill out your copyright notice in the Description page of Project Settings.

#include "MISC/PTStatics.h"

UObject* UPTStatics::GetDefaultObject(UObject* Object)
{
	if (!Object)
		return nullptr;
		
	return Object->GetClass()->GetDefaultObject();
}
