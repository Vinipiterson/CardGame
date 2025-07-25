// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Async/AsyncAction_PushConfirmationDialog.h"
#include "Subsystem/GameplayUserSubsystem.h"

UAsyncAction_PushConfirmationDialog* UAsyncAction_PushConfirmationDialog::PushConfirmationDialog(const UObject* WorldContextObject, EConfirmationDialogType DialogType, FText DialogTitle, FText DialogMessage)
{
	if (!GEngine) return nullptr;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return nullptr;
	
	if (UAsyncAction_PushConfirmationDialog* Node = NewObject<UAsyncAction_PushConfirmationDialog>())
	{
		Node->OwningWorld = World;
		Node->OwningDialogType = DialogType;
		Node->OwningDialogTitle = DialogTitle;
		Node->OwningDialogMessage = DialogMessage;

		Node->RegisterWithGameInstance(World);

		return Node;
	}

	return nullptr;
}

void UAsyncAction_PushConfirmationDialog::Activate()
{
	Super::Activate();

	UGameplayUserSubsystem::Get(OwningWorld.Get())->PushConfirmationDialogAsync(OwningDialogType, 
		OwningDialogTitle, OwningDialogMessage,
		[this](EConfirmationDialogResult ClickedButtonType)
		{
			OnClicked.Broadcast(ClickedButtonType);
			SetReadyToDestroy();
		}
	);
}
