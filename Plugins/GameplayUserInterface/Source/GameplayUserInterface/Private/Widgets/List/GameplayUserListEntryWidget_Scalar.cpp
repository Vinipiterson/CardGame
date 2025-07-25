// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/List/GameplayUserListEntryWidget_Scalar.h"
#include "AnalogSlider.h"
#include "GameFramework/List/GameplayUserListData_Scalar.h"

UGameplayUserListEntryWidget_Scalar::UGameplayUserListEntryWidget_Scalar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UGameplayUserListEntryWidget_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Slider_SettingValue->OnValueChanged.AddUniqueDynamic(this, &UGameplayUserListEntryWidget_Scalar::OnSliderValueChanged);
	Slider_SettingValue->OnMouseCaptureBegin.AddUniqueDynamic(this, &UGameplayUserListEntryWidget_Scalar::OnSliderMouseCaptureBegin);
}

void UGameplayUserListEntryWidget_Scalar::OnOwningListDataSet(UGameplayUserListData* OwningListData)
{
	Super::OnOwningListDataSet(OwningListData);

	ScalarListData = CastChecked<UGameplayUserListData_Scalar>(OwningListData);

	ECommonNumericType NumericType = ScalarListData->GetDisplayNumericType();
	FCommonNumberFormattingOptions FormattingOptions = ScalarListData->GetNumberFormattingOptions();
	
	float CurrentValue = ScalarListData->GetCurrentValue();
	float SliderMinValue = ScalarListData->GetDisplayValueRange().GetLowerBoundValue();
	float SliderMaxValue = ScalarListData->GetDisplayValueRange().GetUpperBoundValue();
	float SliderStepSize = ScalarListData->GetSliderStepSize();

	NumericText_SettingValue->SetNumericType(NumericType);
	NumericText_SettingValue->FormattingSpecification = FormattingOptions;
	NumericText_SettingValue->SetCurrentValue(CurrentValue);

	Slider_SettingValue->SetMinValue(SliderMinValue);
	Slider_SettingValue->SetMaxValue(SliderMaxValue);
	Slider_SettingValue->SetStepSize(SliderStepSize);
	Slider_SettingValue->SetValue(CurrentValue);
	
}

void UGameplayUserListEntryWidget_Scalar::OnOwningListDataModified(UGameplayUserListData* ModifiedListData, EOptionsListDataModifyReason ModifyReason)
{
	if (ScalarListData)
	{
		float CurrentValue = ScalarListData->GetCurrentValue();
		
		NumericText_SettingValue->SetCurrentValue(CurrentValue);
		Slider_SettingValue->SetValue(CurrentValue);
	}
}

void UGameplayUserListEntryWidget_Scalar::OnToggleEntryWidgetHighlightState_Implementation(bool bShouldHighlight) const
{
	Super::OnToggleEntryWidgetHighlightState_Implementation(bShouldHighlight);
	
	if (bShouldHighlight)
	{
		if (HighlightTextStyleClass)
		{
			NumericText_SettingValue->SetStyle(HighlightTextStyleClass);
		}
	}
	else
	{
		if (UnhighlightTextStyleClass)
		{
			NumericText_SettingValue->SetStyle(UnhighlightTextStyleClass);
		}
	}
}

void UGameplayUserListEntryWidget_Scalar::OnSliderValueChanged(float Value)
{
	if (ScalarListData)
	{
		ScalarListData->SetCurrentValueFromSlider(Value);
	}
}

void UGameplayUserListEntryWidget_Scalar::OnSliderMouseCaptureBegin()
{
	SelectThisEntryWidget();
}
