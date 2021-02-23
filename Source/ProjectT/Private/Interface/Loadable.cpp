// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/Loadable.h"

bool ILoadable::IsLoadAsync_Implementation() const noexcept
{
	return false;
}
