// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "Widgets/Foundation/GameplayUserActivatableWidget.h"
#include "GameplayUserConfirmationDialogWidget.generated.h"

class UGameplayUserConfirmationDialogDescriptor;
class UCommonTextBlock;
class UDynamicEntryBox;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserConfirmationDialogWidget : public UGameplayUserActivatableWidget
{
	GENERATED_BODY()
	
public:
	// Gets called outside of the class when this widget is constructed and before it's pushed to the modal stack.
	void InitConfirmationDialog(UGameplayUserConfirmationDialogDescriptor* Descriptor, TFunction<void(EConfirmationDialogResult)> ResultCallback);

	//~Begin UCommonActivatableWidget interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~End of UCommonActivatableWidget interface

private:
	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> Text_DialogTitle;

	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> Text_DialogMessage;

	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UDynamicEntryBox> EntryBox_DialogButtons;
};

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserConfirmationDialogDescriptor : public UObject
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FText DialogTitle;

	UPROPERTY(Transient)
	FText DialogMessage;

	UPROPERTY(Transient)
	TArray<FConfirmationDialogInfo> AvailableDialogButtons;

public:
	static UGameplayUserConfirmationDialogDescriptor* CreateOkDialog(const FText& InDialogTitle, const FText& InDialogMessage);
	static UGameplayUserConfirmationDialogDescriptor* CreateYesNoDialog(const FText& InDialogTitle, const FText& InDialogMessage);
	static UGameplayUserConfirmationDialogDescriptor* CreateOkCancelDialog(const FText& InDialogTitle, const FText& InDialogMessage);

	FORCEINLINE FText GetDialogTitle() const { return DialogTitle; }
	FORCEINLINE FText GetDialogMessage() const { return DialogMessage; }
	FORCEINLINE TArray<FConfirmationDialogInfo> GetAvailableDialogButtons() const { return AvailableDialogButtons; }
};
