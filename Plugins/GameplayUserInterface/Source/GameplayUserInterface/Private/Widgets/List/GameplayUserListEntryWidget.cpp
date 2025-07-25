// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/List/GameplayUserListEntryWidget.h"

#include "CommonButtonBase.h"
#include "GameFramework/List/GameplayUserListData.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"

UGameplayUserListEntryWidget::UGameplayUserListEntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetVisibility(ESlateVisibility::Visible);
}

void UGameplayUserListEntryWidget::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	// Call K2_OnListEntryWidgetHovered with the correct selection state.
	K2_OnListEntryWidgetHovered(bWasHovered, GetListItem() ? IsListItemSelected() : false);

	// Determine the highlight state based on hover and selection.
	const bool bShouldBeHighlighted = bWasHovered || (GetListItem() && IsListItemSelected());
	OnToggleEntryWidgetHighlightState(bShouldBeHighlighted);
}

void UGameplayUserListEntryWidget::ToggleHighlightState(bool bShouldHighlight)
{
	OnToggleEntryWidgetHighlightState(bShouldHighlight);
}

void UGameplayUserListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (UGameplayUserListData* CurrentListData = Cast<UGameplayUserListData>(ListItemObject))
	{
		OnOwningListDataSet(CurrentListData);
	}
}

void UGameplayUserListEntryWidget::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	NativeOnListEntryWidgetHovered(false);
}

void UGameplayUserListEntryWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	OnToggleEntryWidgetHighlightState(bIsSelected);
}

FReply UGameplayUserListEntryWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (!CommonInputSubsystem || CommonInputSubsystem->GetCurrentInputType() != ECommonInputType::Gamepad)
	{
		return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	}

	UWidget* WidgetToFocus = K2_GetWidgetToFocusForGamepad();
	if (!WidgetToFocus)
	{
		return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	}

	TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget();
	if (!SlateWidgetToFocus)
	{
		return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	}
	
	return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
}

void UGameplayUserListEntryWidget::OnToggleEntryWidgetHighlightState_Implementation(bool bShouldHighlight) const
{
	if (bShouldHighlight)
	{
		if (HighlightTextStyleClass)
		{
			Text_SettingDisplayName->SetStyle(HighlightTextStyleClass);
		}
	}
	else
	{
		if (UnhighlightTextStyleClass)
		{
			Text_SettingDisplayName->SetStyle(UnhighlightTextStyleClass);
		}
	}
}

void UGameplayUserListEntryWidget::OnOwningListDataSet(UGameplayUserListData* OwningListData)
{
	if (Text_SettingDisplayName)
	{
		FText SettingDisplayName = OwningListData->GetDisplayName();
		Text_SettingDisplayName->SetText(SettingDisplayName);
	}

	if (!OwningListData->OnListDataModified.IsBoundToObject(this))
	{
		OwningListData->OnListDataModified.AddUObject(this, &UGameplayUserListEntryWidget::OnOwningListDataModified);
	}

	if (!OwningListData->OnDependencyModified.IsBoundToObject(this))
	{
		OwningListData->OnDependencyModified.AddUObject(this, &UGameplayUserListEntryWidget::OnOwningDependencyModified);
	}
	
	OnToggleEditableState(OwningListData->EvaluateEditConditions());

	CachedOwningListData = OwningListData;
}

void UGameplayUserListEntryWidget::OnOwningDependencyModified(UGameplayUserListData* ModifiedDependency, EOptionsListDataModifyReason ModifyReason)
{
	if (!CachedOwningListData) return;
	
	OnToggleEditableState(CachedOwningListData->EvaluateEditConditions());
}

void UGameplayUserListEntryWidget::OnToggleEditableState(bool bIsEditable)
{
	if (Text_SettingDisplayName)
	{
		Text_SettingDisplayName->SetIsEnabled(bIsEditable);
	}
}

void UGameplayUserListEntryWidget::SelectThisEntryWidget()
{
	UListView* OwningListView = CastChecked<UListView>(GetOwningListView());
	
	OwningListView->SetSelectedItem(GetListItem());
}
