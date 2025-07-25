// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Foundation/GameplayUserPrimaryLayoutWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserPrimaryLayout, Log, All);

UCommonActivatableWidgetContainerBase* UGameplayUserPrimaryLayoutWidget::FindWidgetStackByTag(const FGameplayTag& InStackTag) const
{
	checkf(RegisteredStackLayersMap.Contains(InStackTag), TEXT("Can not find the widget stack by the tag %s"), *InStackTag.ToString());

	return RegisteredStackLayersMap.FindRef(InStackTag);
}

void UGameplayUserPrimaryLayoutWidget::RegisterStack(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* Stack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredStackLayersMap.Contains(LayerTag))
		{
			RegisteredStackLayersMap.Add(LayerTag, Stack);

			UE_LOG(LogGameplayUserPrimaryLayout, Log, TEXT("Layer Stack Registered under the tag %s"), *LayerTag.ToString());
		}
	}
}
