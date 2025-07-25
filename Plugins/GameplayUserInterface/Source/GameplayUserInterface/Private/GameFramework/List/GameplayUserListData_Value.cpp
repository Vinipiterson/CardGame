// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/List/GameplayUserListData_Value.h"

void UGameplayUserListData_Value::SetDynamicGetter(const TSharedPtr<FGameplayUserOptionsDataAccessor>& InDynamicGetter)
{
	DataDynamicGetter = InDynamicGetter;
}

void UGameplayUserListData_Value::SetDynamicSetter(const TSharedPtr<FGameplayUserOptionsDataAccessor>& InDynamicSetter)
{
	DataDynamicSetter = InDynamicSetter;
}
