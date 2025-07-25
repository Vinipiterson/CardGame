// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonNumericTextBlock.h"
#include "GameplayUserListData_Value.h"
#include "GameplayUserListData_Scalar.generated.h"

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_Scalar : public UGameplayUserListData_Value
{
	GENERATED_BODY()

public:
	UGameplayUserListData_Scalar();

	LIST_DATA_ACCESSOR(TRange<float>, DisplayValueRange);
	LIST_DATA_ACCESSOR(TRange<float>, OutputValueRange);
	LIST_DATA_ACCESSOR(float, SliderStepSize);
	LIST_DATA_ACCESSOR(ECommonNumericType, DisplayNumericType);
	LIST_DATA_ACCESSOR(FCommonNumberFormattingOptions, NumberFormattingOptions);

	static FCommonNumberFormattingOptions NoDecimalFormattingOptions();
	static FCommonNumberFormattingOptions WithDecimalFormattingOptions(int32 NumFractionalDigits);

	float GetCurrentValue() const;
	void SetCurrentValueFromSlider(float NewValue);

	//~Begin UGameplayUserListData interface
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	virtual void OnEditDependencyModified(UGameplayUserListData* ModifyDependency, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified) override;
	//~End of UGameplayUserListData interface
	
private:
	TRange<float> DisplayValueRange;
	TRange<float> OutputValueRange;
	
	float SliderStepSize = 0.1f;
	
	ECommonNumericType DisplayNumericType;
	
	FCommonNumberFormattingOptions NumberFormattingOptions;

private:
	float StringToFloat(const FString& InString) const;
};
