// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/List/GameplayUserListData_Collection.h"

UGameplayUserListData_Collection::UGameplayUserListData_Collection()
{
	
}

TArray<UGameplayUserListData*> UGameplayUserListData_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UGameplayUserListData_Collection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}

void UGameplayUserListData_Collection::AddChildListData(UGameplayUserListData* InChildListData)
{
	// Notify the child list data to init itself
	InChildListData->InitData();
	
	// Set the child list data's parent to this
	InChildListData->SetParentData(this);
	
	ChildListDataArray.Add(InChildListData);
}
