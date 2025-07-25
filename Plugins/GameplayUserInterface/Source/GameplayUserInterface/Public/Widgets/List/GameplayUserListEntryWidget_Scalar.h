// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayUserListEntryWidget.h"
#include "GameplayUserListEntryWidget_Scalar.generated.h"

class UGameplayUserListData_Scalar;
class UAnalogSlider;
class UCommonNumericTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserListEntryWidget_Scalar : public UGameplayUserListEntryWidget
{
	GENERATED_BODY()

public:
	UGameplayUserListEntryWidget_Scalar(const FObjectInitializer& ObjectInitializer);

protected:
	//~Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	//~End of UUserWidget interface

	//~Begin UGameplayUserListEntryWidget interface
	virtual void OnOwningListDataSet(UGameplayUserListData* OwningListData) override;
	virtual void OnOwningListDataModified(UGameplayUserListData* ModifiedListData, EOptionsListDataModifyReason ModifyReason) override;
	virtual void OnToggleEntryWidgetHighlightState_Implementation(bool bShouldHighlight) const override;
	//~End of UGameplayUserListEntryWidget interface
	
private:
	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonNumericTextBlock> NumericText_SettingValue;

	UPROPERTY(BlueprintReadOnly, Category="Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UAnalogSlider> Slider_SettingValue;

	UPROPERTY(Transient)
	TObjectPtr<UGameplayUserListData_Scalar> ScalarListData;

private:
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnSliderMouseCaptureBegin();
};
