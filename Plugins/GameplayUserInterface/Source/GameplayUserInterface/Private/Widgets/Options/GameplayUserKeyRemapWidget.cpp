// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Options/GameplayUserKeyRemapWidget.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "CommonRichTextBlock.h"
#include "CommonUITypes.h"
#include "ICommonInputModule.h"
#include "Framework/Application/IInputProcessor.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserKeyRemapWidget, Log, All);

class FKeyRemapScreenInputPreProcessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputPreProcessor(ECommonInputType InInputTypeToListenTo, ULocalPlayer* InLocalPlayer)
		: InputTypeToListenTo(InInputTypeToListenTo)
		, LocalPlayer(InLocalPlayer)
	{
	}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedSignature, const FKey& /*PressedKey*/);
	FOnInputPreProcessorKeyPressedSignature OnInputPreProcessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectedCancelledSignature, const FString& /*CancelledReason*/)
	FOnInputPreProcessorKeySelectedCancelledSignature OnInputPreProcessorKeySelectedCancelled;
	
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());

		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());

		return true;
	}

	void ProcessPressedKey(const FKey& InPressedKey)
	{
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectedCancelled.ExecuteIfBound(TEXT("Key Remap has been cancelled"));
			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer.Get());
		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
		
		switch (InputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:
			if (InPressedKey.IsGamepadKey() && CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreProcessorKeySelectedCancelled.ExecuteIfBound(TEXT("Detected Gamepad Button Pressed for Keyboard Inputs. Key Remap has been cancelled."));
				return;
			}
			break;
		case ECommonInputType::Gamepad:
			if (CurrentInputType == ECommonInputType::Gamepad && InPressedKey == EKeys::LeftMouseButton)
			{
				FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));
				check(InputActionData);
				
				OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());

				return;
			}
			
			if (!InPressedKey.IsGamepadKey() )
			{
				OnInputPreProcessorKeySelectedCancelled.ExecuteIfBound(TEXT("Detected non Gamepad Button Pressed for Gamepad Inputs. Key Remap has been cancelled."));
				return;
			}
			break;
		default:
			break;
		}

		OnInputPreProcessorKeyPressed.ExecuteIfBound(InPressedKey);
	}

private:
	ECommonInputType InputTypeToListenTo;
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;
};

void UGameplayUserKeyRemapWidget::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	DesiredInputType = InDesiredInputType;
}

void UGameplayUserKeyRemapWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	// Create and Allocate in Memory an Input Preprocessor to Capture Any Key on this Widget Screen is Activated.
	InputPreProcessor = MakeShared<FKeyRemapScreenInputPreProcessor>(DesiredInputType, GetOwningLocalPlayer());

	// Listen to InputPreProcessor delegates for Input Keys.
	InputPreProcessor->OnInputPreProcessorKeyPressed.BindUObject(this, &UGameplayUserKeyRemapWidget::NativeOnValidKeyPressedDetected);
	InputPreProcessor->OnInputPreProcessorKeySelectedCancelled.BindUObject(this, &UGameplayUserKeyRemapWidget::NativeOnKeySelectedCancelled);

	// Register the Input Preprocessor.
	FSlateApplication::Get().RegisterInputPreProcessor(InputPreProcessor, -1);

	FString InputDeviceName;

	switch (DesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("Mouse & Keyboard");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("Gamepad");
		break;
	default:
		break;
	}

	const FString DisplayRichMessage = FString::Printf(TEXT("<KeyRemap_Default>Press Any</> <KeyRemap_Highlight>%s</> <KeyRemap_Default>Key.</>"), *InputDeviceName);

	RichText_RemapMessage->SetText(FText::FromString(DisplayRichMessage));
}

void UGameplayUserKeyRemapWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (InputPreProcessor)
	{
		// Unregister the Input Preprocessor.
		FSlateApplication::Get().UnregisterInputPreProcessor(InputPreProcessor);

		// Clear cached value.
		InputPreProcessor.Reset();
	}
}

void UGameplayUserKeyRemapWidget::NativeOnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactivateWidget([this, PressedKey]()
	{
		UE_LOG(LogGameplayUserKeyRemapWidget, Warning, TEXT("Pressed Key: %s"), *PressedKey.ToString());
		
		OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		K2_OnValidKeyPressedDetected(PressedKey);
	});
}

void UGameplayUserKeyRemapWidget::NativeOnKeySelectedCancelled(const FString& CancelledReason)
{
	RequestDeactivateWidget([this, CancelledReason]()
	{
		UE_LOG(LogGameplayUserKeyRemapWidget, Warning, TEXT("Pressed Key: %s"), *CancelledReason);
		
		OnKeyRemapScreenKeySelectedCancelled.ExecuteIfBound(CancelledReason);
		K2_OnKeySelectedCancelled(CancelledReason);
	});
}

void UGameplayUserKeyRemapWidget::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	// Delay a tick to make sure the input is processed correctly.
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this, PreDeactivateCallback](float DeltaTime)->bool
	{
		PreDeactivateCallback();
		DeactivateWidget();
		return false;
	}));
}
