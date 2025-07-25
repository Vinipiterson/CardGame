// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Foundation/GameplayUserActivatableWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "GameFramework/GameplayUserPolicy.h"
#include "GameFramework/GameplayUserPlayerController.h"
#include "Input/UIActionBindingHandle.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystem/GameplayUserSubsystem.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#endif

#define LOCTEXT_NAMESPACE "GameplayUserActivatableWidget"

UGameplayUserActivatableWidget::UGameplayUserActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputMode = EActivatableInputMode::Menu;
	MouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	bHidesCursorDuringViewportCapture = true;
}

void UGameplayUserActivatableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGameplayUserActivatableWidget::NativeDestruct()
{
	UnregisterAllBindings();

	Super::NativeDestruct();
}

void UGameplayUserActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	// Show Animation (Fade)
}

void UGameplayUserActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

TOptional<FUIInputConfig> UGameplayUserActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputMode)
	{
		case EActivatableInputMode::GameAndMenu:
			return FUIInputConfig(ECommonInputMode::All, MouseCaptureMode, bHidesCursorDuringViewportCapture);
		break;
		case EActivatableInputMode::Game:
			return FUIInputConfig(ECommonInputMode::Game, MouseCaptureMode, bHidesCursorDuringViewportCapture);
		break;
		case EActivatableInputMode::Menu:
			return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		break;
		default: return TOptional<FUIInputConfig>();
	}
}

#if WITH_EDITOR
void UGameplayUserActivatableWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);
}
#endif

void UGameplayUserActivatableWidget::RegisterBinding(FDataTableRowHandle InputAction, const FOnInputActionExecutedSignature& Callback, FUIActionBindingHandle& BindingHandle)
{
	FBindUIActionArgs BindArgs(InputAction, FSimpleDelegate::CreateLambda([InputAction, Callback]()
	{
		Callback.ExecuteIfBound(InputAction.RowName);
	}));
	BindArgs.bDisplayInActionBar = true;

	BindingHandle = RegisterUIActionBinding(BindArgs);
	BindingHandles.Add(BindingHandle);
}

void UGameplayUserActivatableWidget::UnregisterBinding(FUIActionBindingHandle BindingHandle)
{
	if (BindingHandle.IsValid())
	{
		BindingHandle.Unregister();
		BindingHandles.Remove(BindingHandle);
	}
}

void UGameplayUserActivatableWidget::UnregisterAllBindings()
{
	for (FUIActionBindingHandle Handle : BindingHandles)
	{
		if (Handle.IsValid())
		{
			Handle.Unregister();
		}
	}
	BindingHandles.Empty();
}

AGameplayUserPlayerController* UGameplayUserActivatableWidget::GetOwningGameplayUserPlayer()
{
	if (!OwningGameplayUserPC.IsValid())
	{
		OwningGameplayUserPC = GetOwningPlayer<AGameplayUserPlayerController>();
	}

	return OwningGameplayUserPC.IsValid() ? OwningGameplayUserPC.Get() : nullptr;
}

#undef LOCTEXT_NAMESPACE
