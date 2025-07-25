// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Foundation/GameplayUserRotator.h"
#include "CommonTextBlock.h"

void UGameplayUserRotator::SetSelectedOptionByText(const FText& TextOption)
{
	const int32 FoundTextIndex = TextLabels.IndexOfByPredicate(
		[TextOption](const FText& TextItem)->bool
		{
			return TextItem.EqualTo(TextOption);
		}
	);

	if (FoundTextIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundTextIndex);
		return;
	}

	MyText->SetText(TextOption);
}

bool UGameplayUserRotator::Initialize()
{
	if (Super::Initialize())
	{
		OnCustomNavigation.BindUObject(this, &UGameplayUserRotator::NativeOnCustomNavigation);

		return true;
	}

	return false;
}

void UGameplayUserRotator::NativePreConstruct()
{
	Super::NativePreConstruct();
}

FNavigationReply UGameplayUserRotator::NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	switch (InNavigationEvent.GetNavigationType())
	{
	case EUINavigation::Left:
	case EUINavigation::Right:
		return FNavigationReply::Custom(OnCustomNavigation);
	default:
		return InDefaultReply;
	}
}

void UGameplayUserRotator::SetTextStyle(TSubclassOf<UCommonTextStyle> NewStyle)
{
	if (MyText)
	{
		MyText->SetStyle(NewStyle);
	}
}

TSharedPtr<SWidget> UGameplayUserRotator::NativeOnCustomNavigation(EUINavigation UINavigation)
{
	const int32 Count = TextLabels.Num();

	const int32 CurrentIndex = GetSelectedIndex();
	const int32 PreviousIndex = GetSelectedIndex() - 1;
	const int32 NextIndex = GetSelectedIndex() + 1;

	if (UINavigation == EUINavigation::Left)
	{
		if (CurrentIndex > 0)
		{
			ShiftTextLeftInternal(true);
		}
	}
	else if (UINavigation == EUINavigation::Right)
	{
		if (CurrentIndex < Count - 1)
		{
			ShiftTextRightInternal(true);
		}
	}

	return nullptr;
}
