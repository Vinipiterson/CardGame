// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Options/GameplayUserOptionsDetailsViewWidget.h"
#include "GameFramework/List/GameplayUserListData.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"

void UGameplayUserOptionsDetailsViewWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();
}

void UGameplayUserOptionsDetailsViewWidget::UpdateDetailsViewInfo(UGameplayUserListData* InListData, const FString& InEntryWidgetClassName)
{
	if (!IsValid(InListData))
	{
		return;
	}

	Text_Title->SetText(InListData->GetDisplayName());

	if (!InListData->GetDescriptionImage().IsNull())
	{
		Image_Description->SetBrushFromLazyTexture(InListData->GetDescriptionImage());
		Image_Description->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Image_Description->SetVisibility(ESlateVisibility::Collapsed);
	}

	RichText_Description->SetText(InListData->GetDescriptionText());
	
	const FString DynamicDetails = FString::Printf(TEXT("List Data Class: <Bold>%s</>\nEntry Widget Class: <Bold>%s</>"), *InListData->GetClass()->GetName(), *InEntryWidgetClassName);
	RichText_DynamicDetails->SetText(FText::FromString(DynamicDetails));

	RichText_Disabled->SetText(InListData->EvaluateEditConditions() ? FText::GetEmpty() : InListData->GetDisabledText());
}

void UGameplayUserOptionsDetailsViewWidget::ClearDetailsViewInfo()
{
	Text_Title->SetText(FText::GetEmpty());

	Image_Description->SetVisibility(ESlateVisibility::Collapsed);

	RichText_Description->SetText(FText::GetEmpty());
	RichText_DynamicDetails->SetText(FText::GetEmpty());
	RichText_Disabled->SetText(FText::GetEmpty());
}
