// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayUserListEntryWidget.h"
#include "GameplayUserListEntryWidget_KeyRemap.generated.h"

class UGameplayUserListData_KeyRemap;
class UGameplayUserButtonBase;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserListEntryWidget_KeyRemap : public UGameplayUserListEntryWidget
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	//~End of UUserWidget interface
	
	//~Begin UGameplayUserListEntryWidget interface
	virtual void OnOwningListDataSet(UGameplayUserListData* OwningListData) override;
	virtual void OnOwningListDataModified(UGameplayUserListData* ModifiedListData, EOptionsListDataModifyReason ModifyReason) override;
	virtual void OnToggleEntryWidgetHighlightState_Implementation(bool bShouldHighlight) const override;
	//~End of UGameplayUserListEntryWidget interface

	virtual void OnRemapKeyButtonClicked();
	virtual void OnResetKeyBindingButtonClicked();

	virtual void OnKeyToRemapPressed(const FKey& PressedKey);
	virtual void OnKeyRemapCancelled(const FString& CancelledReason);
	
private:
	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UGameplayUserButtonBase> Button_RemapKey;

	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UGameplayUserButtonBase> Button_ResetKeyBinding;

	UPROPERTY(Transient)
	UGameplayUserListData_KeyRemap* KeyRemapListData;
};
