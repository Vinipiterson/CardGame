// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/List/GameplayUserListData_Scalar.h"
#include "GameplayUserOptionsDataAccessor.h"

UGameplayUserListData_Scalar::UGameplayUserListData_Scalar()
{
	DisplayValueRange = TRange<float>(0.0f, 1.0f);
	OutputValueRange = TRange<float>(0.0f, 1.0f);

	DisplayNumericType = ECommonNumericType::Number;
}

FCommonNumberFormattingOptions UGameplayUserListData_Scalar::NoDecimalFormattingOptions()
{
	FCommonNumberFormattingOptions Result;
	Result.MaximumFractionalDigits = 0;

	return Result;
}

FCommonNumberFormattingOptions UGameplayUserListData_Scalar::WithDecimalFormattingOptions(int32 NumFractionalDigits)
{
	FCommonNumberFormattingOptions Result;
	Result.MaximumFractionalDigits = NumFractionalDigits;

	return Result;
}

float UGameplayUserListData_Scalar::GetCurrentValue() const
{
	if (!DataDynamicGetter)
	{
		return 0.0f;
	}

	const FString StringValue = DataDynamicGetter->GetValueAsString();
	const float ResultValue = StringToFloat(StringValue);
		
	return FMath::GetMappedRangeValueClamped(OutputValueRange, DisplayValueRange, ResultValue);
}

void UGameplayUserListData_Scalar::SetCurrentValueFromSlider(float NewValue)
{
	if (DataDynamicSetter)
	{
		const float Result = FMath::GetMappedRangeValueClamped(DisplayValueRange, OutputValueRange, NewValue);
		DataDynamicSetter->SetValueFromString(LexToString(Result));

		NotifyListDataModified(this);
	}
}

bool UGameplayUserListData_Scalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
	}

	return false;
}

bool UGameplayUserListData_Scalar::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());

			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

			return true;
		}
	}
	
	return false;
}

void UGameplayUserListData_Scalar::OnEditDependencyModified(UGameplayUserListData* ModifyDependency, EOptionsListDataModifyReason ModifyReason)
{
	NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	
	Super::OnEditDependencyModified(ModifyDependency, ModifyReason);
}

float UGameplayUserListData_Scalar::StringToFloat(const FString& InString) const
{
	float Result = 0.0f;
	LexFromString(Result, InString);
	
	return Result;
}
