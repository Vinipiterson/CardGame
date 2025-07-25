// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameplayUserOptionsDetailsViewWidget.generated.h"

class UGameplayUserListData;
class UCommonRichTextBlock;
class UCommonLazyImage;
class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserOptionsDetailsViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(UGameplayUserListData* InListData, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsViewInfo();

protected:
	//~Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	//~End of UUserWidget interface

private:
	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> Text_Title;

	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonLazyImage> Image_Description;

	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonRichTextBlock> RichText_Description;

	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonRichTextBlock> RichText_DynamicDetails;

	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonRichTextBlock> RichText_Disabled;
};
