// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushSoftWidget.generated.h"

class UGameplayUserActivatableWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetSignature, UGameplayUserActivatableWidget*, PushedWidget);

UCLASS()
class GAMEPLAYUSERINTERFACE_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPushSoftWidgetSignature BeforePush;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPushSoftWidgetSignature AfterPush;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Async Action", meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = true, DisplayName = "Push Activatable Widget to Layer Stack"))
	static UAsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<UGameplayUserActivatableWidget> WidgetClass, UPARAM(meta = (Categories = "GameplayUser.LayerStack"))FGameplayTag LayerTag, bool bFocusOnNewlyPushedWidget = true);

	//~Begin UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~End of UBlueprintAsyncActionBase interface
	
private:
	TWeakObjectPtr<UWorld> OwningWorld;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	
	TSoftClassPtr<UGameplayUserActivatableWidget> OwningWidgetClass;
	
	FGameplayTag OwningLayerTag;
	
	bool bFocusOnNewlyPushedWidget;
};
