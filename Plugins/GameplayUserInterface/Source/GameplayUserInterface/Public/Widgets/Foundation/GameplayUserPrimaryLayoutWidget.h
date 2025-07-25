// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "GameplayUserPrimaryLayoutWidget.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserPrimaryLayoutWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InStackTag) const;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Primary Layout")
	void RegisterStack(UPARAM(meta = (Categories = "GameplayUser.LayerStack")) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* Stack);
	
private:
	UPROPERTY()
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredStackLayersMap;
};
