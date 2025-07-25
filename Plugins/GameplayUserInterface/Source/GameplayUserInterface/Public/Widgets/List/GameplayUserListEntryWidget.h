// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Types/GameplayUserSharedTypes.h"
#include "GameplayUserListEntryWidget.generated.h"

class UCommonButtonStyle;
class UCommonTextStyle;
class UGameplayUserListData;
class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserListEntryWidget : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UGameplayUserListEntryWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On List Entry Widget Hovered"))
	void K2_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

	UFUNCTION(BlueprintCallable, Category="Highlight")
	void ToggleHighlightState(bool bShouldHighlight);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Highlight", meta=(AllowPrivateAccess = true))
	TSubclassOf<UCommonTextStyle> HighlightTextStyleClass;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Highlight", meta=(AllowPrivateAccess = true))
	TSubclassOf<UCommonTextStyle> UnhighlightTextStyleClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Highlight", meta=(AllowPrivateAccess = true))
	TSubclassOf<UCommonButtonStyle> HighlightButtonStyleClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Highlight", meta=(AllowPrivateAccess = true))
	TSubclassOf<UCommonButtonStyle> UnhighlightButtonStyleClass;
	
protected:
	//~Begin IUserObjectListEntry interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	//~End of IUserObjectListEntry interface

	//~Begin UUserWidget interface
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//~End of UUserWidget interface

	// The child widget blueprint should override this function for the gamepad interaction to function properly.
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay User List Entry", meta=(DisplayName = "Get Widget to Focus for Gamepad"))
	UWidget* K2_GetWidgetToFocusForGamepad() const;

	// The child widget blueprint should override it to handle the highlight state when this entry widget is hovered or selected.
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay User List Entry", meta=(DisplayName = "On Toggle Entry Widget Highlight State"))
	void OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const;

	// The child class should override this function to handle the initialization needed. Super call is expected.
	virtual void OnOwningListDataSet(UGameplayUserListData* OwningListData);

	// The child class should override this function to update the UI values after the data object has been modified. Super call is not needed.
	virtual void OnOwningListDataModified(UGameplayUserListData* ModifiedListData, EOptionsListDataModifyReason ModifyReason) { }

	virtual void OnOwningDependencyModified(UGameplayUserListData* ModifiedDependency, EOptionsListDataModifyReason ModifyReason);

	// The child class should override this to change editable state of the widgets it owns. Super call is expected.
	virtual void OnToggleEditableState(bool bIsEditable);
	
	void SelectThisEntryWidget();

private:
	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> Text_SettingDisplayName;

	UPROPERTY(Transient)
	TObjectPtr<UGameplayUserListData> CachedOwningListData;
};
