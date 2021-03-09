#pragma once

#include "MISC/AsyncLoad.h"

template <class Func>
static void LoadDataTable(const TSoftObjectPtr<class UDataTable>& DataTable, Func&& Fn, bool bLoadAsync)
{
	if (bLoadAsync)
	{
		AsyncLoad(DataTable, [Fn = Forward<Func>(Fn)](auto DataTable) { Fn(DataTable); });
	}
	else
	{
		DataTable.LoadSynchronous();
		Fn(DataTable);
	}
}

template <class T, class Func>
static void GetData(const TSoftObjectPtr<class UDataTable>& DataTable, int32 Key, Func&& Fn, bool bLoadAsync)
{
	static const T DefaultData;

	const auto OnLoadDataTable = [Key, Fn = Forward<Func>(Fn)](const auto& DataTable)
	{
		const auto* Data = &DefaultData;

		if (DataTable.IsValid())
		{
			const auto* TempData = DataTable.Get()->FindRow
				<T>(FName{ *FString::FromInt(Key) }, TEXT(""));

			if (TempData) Data = TempData;
		}

		Fn(*Data);
	};

	LoadDataTable(DataTable, OnLoadDataTable, bLoadAsync);
}
