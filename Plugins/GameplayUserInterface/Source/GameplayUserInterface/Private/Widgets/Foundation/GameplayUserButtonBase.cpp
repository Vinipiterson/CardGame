// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Foundation/GameplayUserButtonBase.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Subsystem/GameplayUserSubsystem.h"

UGameplayUserButtonBase::UGameplayUserButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bToggleHighlightWhenHovered(false)
	, HighlightButtonStyleClass(nullptr)
	, UnhighlightButtonStyleClass(nullptr)
{
	
}

void UGameplayUserButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UGameplayUserButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (Text_Button && GetCurrentTextStyleClass())
	{
		Text_Button->SetStyle(GetCurrentTextStyleClass());
	}
}

void UGameplayUserButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		UGameplayUserSubsystem::Get(this)->NotifyDescriptionChanged(this, ButtonDescriptionText);
	}

	if (HighlightButtonStyleClass && bToggleHighlightWhenHovered)
	{
		SetStyle(HighlightButtonStyleClass);
	}
}

void UGameplayUserButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	UGameplayUserSubsystem::Get(this)->NotifyDescriptionChanged(this, FText::GetEmpty());

	if (UnhighlightButtonStyleClass && bToggleHighlightWhenHovered)
	{
		SetStyle(UnhighlightButtonStyleClass);
	}
}

void UGameplayUserButtonBase::SetButtonText(FText NewText)
{
	if (Text_Button && !NewText.IsEmpty())
	{
		Text_Button->SetText(NewText);
		Text_Button->SetTextCase(bUseUppercaseForButtonText);
	}
}

FText UGameplayUserButtonBase::GetButtonText() const
{
	if (Text_Button)
	{
		return Text_Button->GetText();
	}

	return FText::GetEmpty();
}

void UGameplayUserButtonBase::SetButtonBrush(const FSlateBrush& NewBrush)
{
	if (Image_Button)
	{
		Image_Button->SetBrush(NewBrush);
	}
}

FSlateBrush UGameplayUserButtonBase::GetButtonBrush() const
{
	if (Image_Button)
	{
		return Image_Button->GetBrush();
	}

	return FSlateBrush();
}
