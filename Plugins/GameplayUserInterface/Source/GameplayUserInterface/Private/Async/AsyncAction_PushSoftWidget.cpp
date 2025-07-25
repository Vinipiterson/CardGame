// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Async/AsyncAction_PushSoftWidget.h"
#include "Subsystem/GameplayUserSubsystem.h"
#include "Widgets/Foundation/GameplayUserActivatableWidget.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<UGameplayUserActivatableWidget> WidgetClass, FGameplayTag LayerTag, bool bFocusOnNewlyPushedWidget)
{
	checkf(!WidgetClass.IsNull(), TEXT("PushSoftWidgetToStack: Was passed a null soft widget class."));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			Node->OwningWorld = World;
			Node->OwningPlayerController = OwningPlayerController;
			Node->OwningWidgetClass = WidgetClass;
			Node->OwningLayerTag = LayerTag;
			Node->bFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
			
			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	Super::Activate();

	const UGameplayUserSubsystem* GameplayUserInterfaceSubsystem = UGameplayUserSubsystem::Get(OwningWorld.Get());
	if (IsValid(GameplayUserInterfaceSubsystem))
	{
		GameplayUserInterfaceSubsystem->PushSoftWidgetToStackAsync(OwningLayerTag, OwningWidgetClass, 
		[this](EAsyncPushWidgetState PushState, UGameplayUserActivatableWidget* PushedWidget)
		{
			switch (PushState)
			{
			case EAsyncPushWidgetState::AfterPush:
				PushedWidget->SetOwningPlayer(OwningPlayerController.Get());
				BeforePush.Broadcast(PushedWidget);
			break;
			case EAsyncPushWidgetState::BeforePush:
				AfterPush.Broadcast(PushedWidget);
				if (bFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}
				SetReadyToDestroy();
			break;
			default:
				break;
			}
		});
	}
}
