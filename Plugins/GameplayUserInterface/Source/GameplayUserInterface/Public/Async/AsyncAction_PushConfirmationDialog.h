// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Types/GameplayUserSharedTypes.h"
#include "AsyncAction_PushConfirmationDialog.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmationDialogClickedSignature, EConfirmationDialogResult, Result);

UCLASS()
class GAMEPLAYUSERINTERFACE_API UAsyncAction_PushConfirmationDialog : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Async Action")
	FOnConfirmationDialogClickedSignature OnClicked;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Async Action", meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = true, DisplayName = "Show Confirmation Dialog"))
	static UAsyncAction_PushConfirmationDialog* PushConfirmationDialog(const UObject* WorldContextObject, EConfirmationDialogType DialogType, FText DialogTitle, FText DialogMessage);

	//~Begin UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~End of UBlueprintAsyncActionBase interface
	
private:
	TWeakObjectPtr<UWorld> OwningWorld;
	
	EConfirmationDialogType OwningDialogType;
	
	FText OwningDialogTitle;
	FText OwningDialogMessage;
};
