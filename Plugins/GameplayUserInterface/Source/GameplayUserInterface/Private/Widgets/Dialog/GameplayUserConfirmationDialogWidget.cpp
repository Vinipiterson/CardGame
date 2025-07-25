// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Dialog/GameplayUserConfirmationDialogWidget.h"
#include "Widgets/Foundation/GameplayUserButtonBase.h"
#include "Types/GameplayUserSharedTypes.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "ICommonInputModule.h"

#define LOCTEXT_NAMESPACE "GameplayUserConfirmationDialogWidget"

void UGameplayUserConfirmationDialogWidget::InitConfirmationDialog(UGameplayUserConfirmationDialogDescriptor* Descriptor, TFunction<void(EConfirmationDialogResult)> ResultCallback)
{
	check(Descriptor && Text_DialogTitle && Text_DialogMessage);

	Text_DialogTitle->SetText(Descriptor->GetDialogTitle());
	Text_DialogMessage->SetText(Descriptor->GetDialogMessage());

	// Checking if the entry box has old buttons created previously.
	if (EntryBox_DialogButtons->GetNumEntries() != 0)
	{
		/**
		 * Clearing the old buttons the entry box has. The widget type for the entry box,
		 * is specified in the child widget blueprint.
		 */
		EntryBox_DialogButtons->Reset<UGameplayUserButtonBase>([](UGameplayUserButtonBase& ExistingButton)
		{
			ExistingButton.OnClicked().Clear();
		});
	}

	check(!Descriptor->GetAvailableDialogButtons().IsEmpty());

	for (const FConfirmationDialogInfo& AvailableButtonInfo : Descriptor->GetAvailableDialogButtons())
	{
		UGameplayUserButtonBase* AddedButton = EntryBox_DialogButtons->CreateEntry<UGameplayUserButtonBase>();
		AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
		AddedButton->OnClicked().AddLambda([this, ResultCallback, AvailableButtonInfo]()
		{
			ResultCallback(AvailableButtonInfo.DialogResult);
			DeactivateWidget();
		});
	}

	if (EntryBox_DialogButtons->GetNumEntries() != 0)
	{
		/**
		 * Set focus on the last button. So if there are two buttons, one is yes, one is no.
		 * Our gamepad will focus on the No button (last button added).
		 */
		EntryBox_DialogButtons->GetAllEntries().Last()->SetFocus();
	}
}

UWidget* UGameplayUserConfirmationDialogWidget::NativeGetDesiredFocusTarget() const
{
	// Set focus on the last button so Gamepad has an option to start with.
	if (EntryBox_DialogButtons->GetNumEntries() != 0)
	{
		EntryBox_DialogButtons->GetAllEntries().Last()->SetFocus();
	}

	return Super::NativeGetDesiredFocusTarget();
}

UGameplayUserConfirmationDialogDescriptor* UGameplayUserConfirmationDialogDescriptor::CreateOkDialog(const FText& InDialogTitle, const FText& InDialogMessage)
{
	UGameplayUserConfirmationDialogDescriptor* Descriptor = NewObject<UGameplayUserConfirmationDialogDescriptor>();
	if (!ensure(Descriptor)) return nullptr;

	Descriptor->DialogTitle = InDialogTitle;
	Descriptor->DialogMessage = InDialogMessage;

	FConfirmationDialogInfo OkButtonInfo;
	OkButtonInfo.DialogResult = EConfirmationDialogResult::Closed;
	OkButtonInfo.ButtonTextToDisplay = LOCTEXT("ConfirmationDialog_Ok", "Ok");

	Descriptor->AvailableDialogButtons.Add(OkButtonInfo);

	return Descriptor;
}

UGameplayUserConfirmationDialogDescriptor* UGameplayUserConfirmationDialogDescriptor::CreateYesNoDialog(const FText& InDialogTitle, const FText& InDialogMessage)
{
	UGameplayUserConfirmationDialogDescriptor* Descriptor = NewObject<UGameplayUserConfirmationDialogDescriptor>();
	if (!ensure(Descriptor)) return nullptr;

	Descriptor->DialogTitle = InDialogTitle;
	Descriptor->DialogMessage = InDialogMessage;

	FConfirmationDialogInfo YesButtonInfo;
	YesButtonInfo.DialogResult = EConfirmationDialogResult::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = LOCTEXT("ConfirmationDialog_Yes", "Yes");

	FConfirmationDialogInfo NoButtonInfo;
	NoButtonInfo.DialogResult = EConfirmationDialogResult::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = LOCTEXT("ConfirmationDialog_No", "No");

	Descriptor->AvailableDialogButtons.Add(YesButtonInfo);
	Descriptor->AvailableDialogButtons.Add(NoButtonInfo);

	return Descriptor;
}

UGameplayUserConfirmationDialogDescriptor* UGameplayUserConfirmationDialogDescriptor::CreateOkCancelDialog(const FText& InDialogTitle, const FText& InDialogMessage)
{
	UGameplayUserConfirmationDialogDescriptor* Descriptor = NewObject<UGameplayUserConfirmationDialogDescriptor>();
	if (!ensure(Descriptor)) return nullptr;

	Descriptor->DialogTitle = InDialogTitle;
	Descriptor->DialogMessage = InDialogMessage;

	FConfirmationDialogInfo OkButtonInfo;
	OkButtonInfo.DialogResult = EConfirmationDialogResult::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = LOCTEXT("ConfirmationDialog_Ok", "Ok");

	FConfirmationDialogInfo CancelButtonInfo;
	CancelButtonInfo.DialogResult = EConfirmationDialogResult::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = LOCTEXT("ConfirmationDialog_Cancel", "Cancel");

	Descriptor->AvailableDialogButtons.Add(OkButtonInfo);
	Descriptor->AvailableDialogButtons.Add(CancelButtonInfo);

	return Descriptor;
}

#undef LOCTEXT_NAMESPACE
