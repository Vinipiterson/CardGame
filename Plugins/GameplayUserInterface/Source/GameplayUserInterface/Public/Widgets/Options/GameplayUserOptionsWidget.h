// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "Widgets/Foundation/GameplayUserActivatableWidget.h"
#include "GameplayUserOptionsWidget.generated.h"

class UGameplayUserListData;
class UGameplayUserOptionsDetailsViewWidget;
class UGameplayUserTabListWidgetBase;
class UGameplayUserDataRegistry;
class UGameplayUserListView;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick), HideCategories=("Back"))
class GAMEPLAYUSERINTERFACE_API UGameplayUserOptionsWidget : public UGameplayUserActivatableWidget
{
	GENERATED_BODY()
	
public:
	UGameplayUserOptionsWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	//~End of UUserWidget interface

	//~Begin UCommonActivatableWidget interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~End of UCommonActivatableWidget interface

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Actions", meta=(DisplayName = "On Reset Bound Action Button Triggered"))
	void NativeOnResetBoundActionButtonTriggered();

	UFUNCTION(BlueprintNativeEvent, Category = "Actions", meta=(DisplayName = "On Apply Bound Action Button Triggered"))
	void NativeOnApplyBoundActionButtonTriggered();

	UFUNCTION(BlueprintNativeEvent, Category = "Actions", meta=(DisplayName = "On Back Bound Action Button Triggered"))
	void NativeOnBackBoundActionButtonTriggered();

	UFUNCTION(BlueprintCallable, Category = "Options")
	void ApplySettings();	

private:
	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UGameplayUserTabListWidgetBase> TabList_OptionTabs;

	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UGameplayUserListView> ListView_OptionsList;

	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UGameplayUserOptionsDetailsViewWidget> DetailsView_OptionsDetails;
	
	// Handle the creation of data in the options screen. Direct access to this variable is forbidden.
	UPROPERTY(Transient)
	TObjectPtr<UGameplayUserDataRegistry> CreatedOwningDataRegistry;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ApplyAction;

	FUIActionBindingHandle ResetActionBindingHandle;
	FUIActionBindingHandle ApplyActionBindingHandle;

	UPROPERTY(Transient)
	TArray<UGameplayUserListData*> ResettableDataArray;

	bool bIsResettingData = false;

private:
	UGameplayUserDataRegistry* GetOrCreateDataRegistry();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabId);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

	void OnListViewListDataModified(UGameplayUserListData* ModifiedData, EOptionsListDataModifyReason ModifiedReason);
};
