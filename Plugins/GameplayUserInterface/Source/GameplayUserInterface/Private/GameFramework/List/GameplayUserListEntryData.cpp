// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/List/GameplayUserListEntryData.h"
#include "GameFramework/List/GameplayUserListData.h"

TSubclassOf<UGameplayUserListEntryWidget> UGameplayUserListEntryData::FindEntryWidgetClassByListData(UGameplayUserListData* ListData) const
{
	check(ListData);

	for (UClass* ListDataClass = ListData->GetClass(); ListDataClass; ListDataClass = ListDataClass->GetSuperClass())
	{
		if (TSubclassOf<UGameplayUserListData> ConvertedListDataClass = TSubclassOf<UGameplayUserListData>(ListDataClass))
		{
			if (ListEntryDataMap.Contains(ConvertedListDataClass))
			{
				return ListEntryDataMap.FindRef(ConvertedListDataClass);
			}
		}
	}

	return TSubclassOf<UGameplayUserListEntryWidget>();
}
