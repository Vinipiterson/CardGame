// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/List/GameplayUserListEntryWidget_String.h"
#include "Widgets/Foundation/GameplayUserButtonBase.h"
#include "Widgets/Foundation/GameplayUserRotator.h"
#include "GameFramework/List/GameplayUserListData_String.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserListEntryWidget_String, Log, All);

UGameplayUserListEntryWidget_String::UGameplayUserListEntryWidget_String(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGameplayUserListEntryWidget_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_PreviousOption->OnClicked().AddUObject(this, &UGameplayUserListEntryWidget_String::OnPreviousOptionButtonClicked);
	Button_NextOption->OnClicked().AddUObject(this, &UGameplayUserListEntryWidget_String::OnNextOptionButtonClicked);
	Rotator_Options->OnRotatedEvent.AddUObject(this, &UGameplayUserListEntryWidget_String::OnRotatorValueChanged);

	Rotator_Options->OnClicked().AddLambda([this]()
	{
		SelectThisEntryWidget();
	});
}

void UGameplayUserListEntryWidget_String::UpdateRotatorButtons()
{
	if (!StringListData) return;
	
	const int32 SelectedIndex = Rotator_Options->GetSelectedIndex();
	const int32 NumOptions = StringListData->GetAvailableOptionsTextArray().Num();

	const bool bHasNext = SelectedIndex < NumOptions - 1;
	const bool bHasPrevious = SelectedIndex > 0;

	Button_NextOption->SetVisibility(bHasNext ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Button_PreviousOption->SetVisibility(bHasPrevious ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGameplayUserListEntryWidget_String::OnOwningListDataSet(UGameplayUserListData* OwningListData)
{
	Super::OnOwningListDataSet(OwningListData);

	StringListData = CastChecked<UGameplayUserListData_String>(OwningListData);

	TArray<FText> AvailableTexts = StringListData->GetAvailableOptionsTextArray();
	FText CurrentDisplayText = StringListData->GetCurrentDisplayText();
	
	Rotator_Options->PopulateTextLabels(AvailableTexts);
	Rotator_Options->SetSelectedOptionByText(CurrentDisplayText);

	UpdateRotatorButtons();
}

void UGameplayUserListEntryWidget_String::OnOwningListDataModified(UGameplayUserListData* ModifiedListData, EOptionsListDataModifyReason ModifyReason)
{
	if (IsValid(StringListData))
	{
		FText CurrentDisplayText = StringListData->GetCurrentDisplayText();
		Rotator_Options->SetSelectedOptionByText(CurrentDisplayText);
	}
}

void UGameplayUserListEntryWidget_String::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	Button_PreviousOption->SetIsEnabled(bIsEditable);
	Button_NextOption->SetIsEnabled(bIsEditable);
	
	Rotator_Options->SetIsEnabled(bIsEditable);
}

void UGameplayUserListEntryWidget_String::OnToggleEntryWidgetHighlightState_Implementation(bool bShouldHighlight) const
{
	Super::OnToggleEntryWidgetHighlightState_Implementation(bShouldHighlight);

	if (bShouldHighlight)
	{
		if (HighlightTextStyleClass)
		{
			Rotator_Options->SetTextStyle(HighlightTextStyleClass);
		}

		if (HighlightButtonStyleClass)
		{
			Button_PreviousOption->SetStyle(HighlightButtonStyleClass);
			Button_NextOption->SetStyle(HighlightButtonStyleClass);
		}
	}
	else
	{
		if (UnhighlightTextStyleClass)
		{
			Rotator_Options->SetTextStyle(UnhighlightTextStyleClass);
		}

		if (UnhighlightButtonStyleClass)
		{
			Button_PreviousOption->SetStyle(UnhighlightButtonStyleClass);
			Button_NextOption->SetStyle(UnhighlightButtonStyleClass);
		}
	}
}

void UGameplayUserListEntryWidget_String::OnPreviousOptionButtonClicked()
{
	if (IsValid(StringListData))
	{
		StringListData->BackToPreviousOption();
	}

	SelectThisEntryWidget();
	UpdateRotatorButtons();
}

void UGameplayUserListEntryWidget_String::OnNextOptionButtonClicked()
{
	if (IsValid(StringListData))
	{
		StringListData->AdvancedToNextOption();
	}

	SelectThisEntryWidget();
	UpdateRotatorButtons();
}

void UGameplayUserListEntryWidget_String::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!StringListData)
	{
		return;
	}

	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (!CommonInputSubsystem || !bUserInitiated)
	{
		return;
	}

	if (CommonInputSubsystem->GetCurrentInputType() != ECommonInputType::Gamepad)
	{
		return;
	}

	StringListData->OnRotatorInitiatedValueChanged(Rotator_Options->GetSelectedText());

	UpdateRotatorButtons();
}
