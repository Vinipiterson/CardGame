// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/List/GameplayUserListEntryWidget_KeyRemap.h"
#include "Widgets/Foundation/GameplayUserButtonBase.h"
#include "Widgets/Options/GameplayUserKeyRemapWidget.h"
#include "GameFramework/List/GameplayUserListData_KeyRemap.h"
#include "Library/GameplayUserLibrary.h"
#include "Subsystem/GameplayUserSubsystem.h"
#include "GameplayUserTags.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserListEntryWidget_KeyRemap, Log, All);

#define LOCTEXT_NAMESPACE "GameplayUserListEntryWidget_KeyRemap"

void UGameplayUserListEntryWidget_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_RemapKey->OnClicked().AddUObject(this, &UGameplayUserListEntryWidget_KeyRemap::OnRemapKeyButtonClicked);
	Button_ResetKeyBinding->OnClicked().AddUObject(this, &UGameplayUserListEntryWidget_KeyRemap::OnResetKeyBindingButtonClicked);
}

void UGameplayUserListEntryWidget_KeyRemap::OnOwningListDataSet(UGameplayUserListData* OwningListData)
{
	Super::OnOwningListDataSet(OwningListData);

	KeyRemapListData = CastChecked<UGameplayUserListData_KeyRemap>(OwningListData);

	Button_RemapKey->SetButtonBrush(KeyRemapListData->GetIconFromCurrentKey());
}

void UGameplayUserListEntryWidget_KeyRemap::OnOwningListDataModified(UGameplayUserListData* ModifiedListData, EOptionsListDataModifyReason ModifyReason)
{
	if (KeyRemapListData)
	{
		Button_RemapKey->SetButtonBrush(KeyRemapListData->GetIconFromCurrentKey());
	}
}

void UGameplayUserListEntryWidget_KeyRemap::OnToggleEntryWidgetHighlightState_Implementation(bool bShouldHighlight) const
{
	Super::OnToggleEntryWidgetHighlightState_Implementation(bShouldHighlight);

	if (bShouldHighlight)
	{
		if (HighlightButtonStyleClass)
		{
			Button_RemapKey->SetStyle(HighlightButtonStyleClass);
			Button_ResetKeyBinding->SetStyle(HighlightButtonStyleClass);
		}
	}
	else
	{
		if (UnhighlightButtonStyleClass)
		{
			Button_RemapKey->SetStyle(UnhighlightButtonStyleClass);
			Button_ResetKeyBinding->SetStyle(UnhighlightButtonStyleClass);
		}
	}
}

void UGameplayUserListEntryWidget_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();
	
	UGameplayUserSubsystem::Get(this)->PushSoftWidgetToStackAsync(GameplayUserTags::LayerStack::LayerStack_Modal,
		UGameplayUserLibrary::GetGameplayUserScreenByTag(GameplayUserTags::Screen::Screen_KeyRemap),
		[this](EAsyncPushWidgetState PushWidgetState, UGameplayUserActivatableWidget* PushedWidget)
		{
			if (PushWidgetState == EAsyncPushWidgetState::BeforePush)
			{
				UGameplayUserKeyRemapWidget* PushedKeyRemapScreen = CastChecked<UGameplayUserKeyRemapWidget>(PushedWidget);
				PushedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &UGameplayUserListEntryWidget_KeyRemap::OnKeyToRemapPressed);
				PushedKeyRemapScreen->OnKeyRemapScreenKeySelectedCancelled.BindUObject(this, &UGameplayUserListEntryWidget_KeyRemap::OnKeyRemapCancelled);
				
				if (KeyRemapListData)
				{
					PushedKeyRemapScreen->SetDesiredInputTypeToFilter(KeyRemapListData->GetDesiredInputKeyType());
				}
			}
		}
	);
}

void UGameplayUserListEntryWidget_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!KeyRemapListData) return;
	
	// Check if the current key is already the default key. Display OK Screen that says is already the default key to the player.
	if (!KeyRemapListData->CanResetBackToDefaultValue())
	{
		UGameplayUserSubsystem::Get(this)->PushConfirmationDialogAsync(EConfirmationDialogType::Ok,
			LOCTEXT("KeyRemapDialog_ResetKeyMapping_Title", "Reset Key Mapping"),
			FText::Format(LOCTEXT("KeyRemapDialog_ResetKeyMapping_Message", "The key binding for {0} is already set to default."), KeyRemapListData->GetDisplayName()),
			[](EConfirmationDialogResult DialogResult)
			{
				// ...
			}
		);

		return;
	}

	// Reset the key binding back to default.
	UGameplayUserSubsystem::Get(this)->PushConfirmationDialogAsync(EConfirmationDialogType::YesNo,
		LOCTEXT("KeyRemapDialog_ResetKeyBinding_Title", "Reset Key Mapping"),
		FText::Format(LOCTEXT("KeyRemapDialog_ResetKeyBinding_Message", "Are you sure you want to reset the key binding for {0} ?"), KeyRemapListData->GetDisplayName()),
		[this](EConfirmationDialogResult DialogResult)
		{
			if (DialogResult == EConfirmationDialogResult::Confirmed)
			{
				KeyRemapListData->TryResetBackToDefaultValue();
			}
		}
	);
}

void UGameplayUserListEntryWidget_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (KeyRemapListData)
	{
		KeyRemapListData->BindNewKey(PressedKey);
	}
}

void UGameplayUserListEntryWidget_KeyRemap::OnKeyRemapCancelled(const FString& CancelledReason)
{
	UGameplayUserSubsystem::Get(this)->PushConfirmationDialogAsync(EConfirmationDialogType::Ok,
		LOCTEXT("KeyRemapDialog_Cancelled_Title", "Key Remap"),
		FText::Format(LOCTEXT("KeyRemapDialog_Cancelled_Message", "{0}"), FText::FromString(CancelledReason)),
		[](EConfirmationDialogResult DialogResult)
		{
			// ...
		}
	);
}

#undef LOCTEXT_NAMESPACE
