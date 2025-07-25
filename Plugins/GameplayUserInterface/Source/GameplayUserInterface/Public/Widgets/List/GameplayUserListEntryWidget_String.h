// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayUserListEntryWidget.h"
#include "GameplayUserListEntryWidget_String.generated.h"

class UGameplayUserRotator;
class UGameplayUserButtonBase;
class UGameplayUserListData_String;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserListEntryWidget_String : public UGameplayUserListEntryWidget
{
	GENERATED_BODY()

public:
	UGameplayUserListEntryWidget_String(const FObjectInitializer& ObjectInitializer);

protected:
	//~Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	//~End of UUserWidget interface
	
	//~Begin UGameplayUserListEntryWidget interface
	virtual void OnOwningListDataSet(UGameplayUserListData* OwningListData) override;
	virtual void OnOwningListDataModified(UGameplayUserListData* ModifiedListData, EOptionsListDataModifyReason ModifyReason) override;
	virtual void OnToggleEditableState(bool bIsEditable) override;
	virtual void OnToggleEntryWidgetHighlightState_Implementation(bool bShouldHighlight) const override;
	//~End of UGameplayUserListEntryWidget interface


	void UpdateRotatorButtons();

private:
	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UGameplayUserButtonBase> Button_PreviousOption;

	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UGameplayUserRotator> Rotator_Options;

	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UGameplayUserButtonBase> Button_NextOption;

	UPROPERTY(Transient)
	TObjectPtr<UGameplayUserListData_String> StringListData;

private:
	virtual void OnPreviousOptionButtonClicked();
	virtual void OnNextOptionButtonClicked();

	virtual void OnRotatorValueChanged(int32 Value, bool bUserInitiated);
};
