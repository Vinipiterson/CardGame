// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Options/GameplayUserOptionsWidget.h"
#include "Widgets/Options/GameplayUserOptionsDetailsViewWidget.h"
#include "Widgets/Foundation/GameplayUserTabListWidgetBase.h"
#include "Widgets/Foundation/GameplayUserButtonBase.h"
#include "Widgets/List/GameplayUserListView.h"
#include "Widgets/List/GameplayUserListEntryWidget.h"
#include "GameFramework/Registry/GameplayUserDataRegistry.h"
#include "GameFramework/List/GameplayUserListData_Collection.h"
#include "GameFramework/GameplayUserSettings.h"
#include "DeveloperSettings/DeveloperSettings_UserInterface.h"
#include "Subsystem/GameplayUserSubsystem.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "Engine/DataTable.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserOptionsWidget, Log, All);

#define LOCTEXT_NAMESPACE "GameplayUserOptionsWidget"

UGameplayUserOptionsWidget::UGameplayUserOptionsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGameplayUserOptionsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		FBindUIActionArgs Args(ResetAction, true, FSimpleDelegate::CreateUObject(this, &UGameplayUserOptionsWidget::NativeOnResetBoundActionButtonTriggered));
		ResetActionBindingHandle = RegisterUIActionBinding(Args);
	}

	if (!ApplyAction.IsNull() && !GetDefault<UDeveloperSettings_UserInterface>()->bShouldApplySettingsImmediately)
	{
		FBindUIActionArgs Args(ApplyAction, true, FSimpleDelegate::CreateUObject(this, &UGameplayUserOptionsWidget::NativeOnApplyBoundActionButtonTriggered));
		ApplyActionBindingHandle = RegisterUIActionBinding(Args);
	}

	FDataTableRowHandle DefaultBackAction = ICommonInputModule::GetSettings().GetDefaultBackAction();
	
	FBindUIActionArgs Args(DefaultBackAction, true, FSimpleDelegate::CreateUObject(this, &UGameplayUserOptionsWidget::NativeOnBackBoundActionButtonTriggered));
	RegisterUIActionBinding(Args);

	TabList_OptionTabs->OnTabSelected.AddUniqueDynamic(this, &UGameplayUserOptionsWidget::OnOptionsTabSelected);

	ListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &UGameplayUserOptionsWidget::OnListViewItemHovered);
	ListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &UGameplayUserOptionsWidget::OnListViewItemSelected);
}

void UGameplayUserOptionsWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UGameplayUserListData_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredCollectionTabs())
	{
		if (!IsValid(TabCollection))
		{
			continue;
		}

		const FName TabID = TabCollection->GetDevName();
		const FText TabDisplayName = TabCollection->GetDisplayName();
		
		if (TabList_OptionTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}

		TabList_OptionTabs->RequestRegisterTab(TabID, TabDisplayName);
	}
}

void UGameplayUserOptionsWidget::NativeOnDeactivated()
{	
	Super::NativeOnDeactivated();

	UGameplayUserSettings::Get()->ApplySettings(true);
}

UWidget* UGameplayUserOptionsWidget::NativeGetDesiredFocusTarget() const
{
	UObject* SelectedObject = ListView_OptionsList->GetSelectedItem();
	if (!SelectedObject)
	{
		return Super::NativeGetDesiredFocusTarget();
	}
	
	UUserWidget* SelectedEntryWidget = ListView_OptionsList->GetEntryWidgetFromItem(SelectedObject);
	if (!SelectedEntryWidget)
	{
		return Super::NativeGetDesiredFocusTarget();
	}

	return SelectedEntryWidget;
}

void UGameplayUserOptionsWidget::ApplySettings()
{
	UGameplayUserSettings::Get()->ApplySettings(true);
}

void UGameplayUserOptionsWidget::NativeOnResetBoundActionButtonTriggered_Implementation()
{
	UE_LOG(LogGameplayUserOptionsWidget, Log, TEXT("Reset bound action triggered."));

	if (ResettableDataArray.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedTabButton = TabList_OptionTabs->GetTabButtonBaseByID(TabList_OptionTabs->GetActiveTab());
	const FString SelectedTabButtonName = CastChecked<UGameplayUserButtonBase>(SelectedTabButton)->GetButtonText().ToString();

	FText DialogTitle = LOCTEXT("GameplayUserOptionsWidget_ResetDialogTitle", "Reset");
	FText DialogMessage = FText::Format(
		LOCTEXT("GameplayUserOptionsWidget_ResetDialogMessage", "Are you sure you want to reset all the settings under the {0} tab?"), FText::FromString(SelectedTabButtonName)
	);
	
	UGameplayUserSubsystem::Get(this)->PushConfirmationDialogAsync(EConfirmationDialogType::YesNo, 
		DialogTitle, DialogMessage,
		[this](EConfirmationDialogResult DialogResult)
		{
			if (DialogResult != EConfirmationDialogResult::Confirmed)
			{
				return;
			}

			bIsResettingData = true;
			bool bHasDataFailedToReset = false;

			for (UGameplayUserListData* DataToReset : ResettableDataArray)
			{
				if (!DataToReset)
				{
					continue;
				}

				if (DataToReset->TryResetBackToDefaultValue())
				{
					UE_LOG(LogGameplayUserOptionsWidget, Warning, TEXT("%s was reset"), *DataToReset->GetDisplayName().ToString());
				}
				else
				{
					bHasDataFailedToReset = true;
					UE_LOG(LogGameplayUserOptionsWidget, Error, TEXT("%s failed to reset."), *DataToReset->GetDisplayName().ToString());
				}
			}

			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionBindingHandle);
			}

			bIsResettingData = false;
		}
	);
}

void UGameplayUserOptionsWidget::NativeOnApplyBoundActionButtonTriggered_Implementation()
{
	UGameplayUserSettings::Get()->ApplySettings(true);
}

void UGameplayUserOptionsWidget::NativeOnBackBoundActionButtonTriggered_Implementation()
{
	DeactivateWidget();
}

UGameplayUserDataRegistry* UGameplayUserOptionsWidget::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		const UDeveloperSettings_UserInterface* InterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();
		const TSubclassOf<UGameplayUserDataRegistry> ConcreteDataRegistryClass = InterfaceSettings->GameplayUserDataRegistryClass.LoadSynchronous();
		if (ConcreteDataRegistryClass)
		{
			CreatedOwningDataRegistry = NewObject<UGameplayUserDataRegistry>(this, ConcreteDataRegistryClass);
			CreatedOwningDataRegistry->InitRegistry(GetOwningLocalPlayer());
		}
	}

	checkf(CreatedOwningDataRegistry, TEXT("Data Registry for Options Screen is not valid."));

	return CreatedOwningDataRegistry;
}

void UGameplayUserOptionsWidget::OnOptionsTabSelected(FName TabId)
{
	UE_LOG(LogGameplayUserOptionsWidget, Log, TEXT("New Tab Selected - Tab ID: %s"), *TabId.ToString());

	DetailsView_OptionsDetails->ClearDetailsViewInfo();

	TArray<UGameplayUserListData*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);

	ListView_OptionsList->SetListItems<UGameplayUserListData*>(FoundListSourceItems);
	ListView_OptionsList->RequestRefresh();

	if (ListView_OptionsList->GetNumItems() != 0)
	{
		ListView_OptionsList->NavigateToIndex(0);
		ListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UGameplayUserListData* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &UGameplayUserOptionsWidget::OnListViewListDataModified);
		}
		
		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionBindingHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionBindingHandle))
		{
			AddActionBinding(ResetActionBindingHandle);
		}
	}
}

void UGameplayUserOptionsWidget::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!IsValid(InHoveredItem))
	{
		return;
	}

	// UGameplayUserListData* CurrentListData = CastChecked<UGameplayUserListData>(InHoveredItem);
	//
	// const FString DebugString = CurrentListData->GetDataDisplayName().ToString() + TEXT(" was ") + (bWasHovered ? TEXT("hovered") : TEXT("unhovered"));
	// UE_LOG(LogGameplayUserOptionsWidget, Warning, TEXT("%s"), *DebugString);

	UGameplayUserListEntryWidget* HoveredEntryWidget = ListView_OptionsList->GetEntryWidgetFromItem<UGameplayUserListEntryWidget>(InHoveredItem);
	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);
	
	if (bWasHovered)
	{
		UGameplayUserListData* HoveredOption = CastChecked<UGameplayUserListData>(InHoveredItem);
		DetailsView_OptionsDetails->UpdateDetailsViewInfo(HoveredOption, TryGetEntryWidgetClassName(InHoveredItem));
	}
	else
	{
		if (UGameplayUserListData* SelectedItem = ListView_OptionsList->GetSelectedItem<UGameplayUserListData>())
		{
			DetailsView_OptionsDetails->UpdateDetailsViewInfo(SelectedItem, TryGetEntryWidgetClassName(SelectedItem));
		}
	}
}

void UGameplayUserOptionsWidget::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!IsValid(InSelectedItem))
	{
		return;
	}
	
	//UGameplayUserListData* CurrentListData = CastChecked<UGameplayUserListData>(InSelectedItem);
	
	// const FString DebugString = CurrentListData->GetDataDisplayName().ToString() + TEXT(" was selected");
	// UE_LOG(LogGameplayUserOptionsWidget, Warning, TEXT("%s"), *DebugString);

	UGameplayUserListData* SelectedOption = CastChecked<UGameplayUserListData>(InSelectedItem);
	DetailsView_OptionsDetails->UpdateDetailsViewInfo(SelectedOption, TryGetEntryWidgetClassName(InSelectedItem));
}

FString UGameplayUserOptionsWidget::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = ListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}

	return TEXT("Entry Widget Class is not valid.");
}

void UGameplayUserOptionsWidget::OnListViewListDataModified(UGameplayUserListData* ModifiedData, EOptionsListDataModifyReason ModifiedReason)
{
	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionBindingHandle))
		{
			AddActionBinding(ResetActionBindingHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionBindingHandle);
	}
}

#undef LOCTEXT_NAMESPACE