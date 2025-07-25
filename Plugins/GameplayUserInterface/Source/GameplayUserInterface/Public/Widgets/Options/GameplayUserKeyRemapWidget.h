// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonInputTypeEnum.h"
#include "Widgets/Foundation/GameplayUserActivatableWidget.h"
#include "GameplayUserKeyRemapWidget.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreProcessor;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserKeyRemapWidget : public UGameplayUserActivatableWidget
{
	GENERATED_BODY()

public:
	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);

	DECLARE_DELEGATE_OneParam(OnKeyRemapScreenKeyPressedSignature, const FKey& /*PressedKey*/);
	OnKeyRemapScreenKeyPressedSignature OnKeyRemapScreenKeyPressed;

	DECLARE_DELEGATE_OneParam(OnKeyRemapScreenKeySelectedCancelledSignature, const FString& /*CancelledReason*/);
	OnKeyRemapScreenKeySelectedCancelledSignature OnKeyRemapScreenKeySelectedCancelled;
	
protected:
	//~Begin UCommonActivatableWidget interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	//~End of UCommonActivatableWidget interface

	UFUNCTION(BlueprintImplementableEvent, Category="Key Remap", meta=(DisplayName = "On Valid Key Pressed Detected"))
	void K2_OnValidKeyPressedDetected(const FKey& PressedKey);
	virtual void NativeOnValidKeyPressedDetected(const FKey& PressedKey);

	UFUNCTION(BlueprintImplementableEvent, Category="Key Remap", meta=(DisplayName = "On Key Selected Cancelled"))
	void K2_OnKeySelectedCancelled(const FString& CancelledReason);
	virtual void NativeOnKeySelectedCancelled(const FString& CancelledReason);
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonRichTextBlock> RichText_RemapMessage;

	TSharedPtr<FKeyRemapScreenInputPreProcessor> InputPreProcessor;

	ECommonInputType DesiredInputType;

private:
	// Delay a tick to make sure the input key is captured properly before calling the PreDeactivateCallback and deactivating the widget.
	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);
};
