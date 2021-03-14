#pragma once

#include "MISC/AsyncLoad.h"

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

	LoadObject(DataTable, OnLoadDataTable, bLoadAsync);
}
