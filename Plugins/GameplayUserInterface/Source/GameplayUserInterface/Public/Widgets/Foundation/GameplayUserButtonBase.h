// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonButtonBase.h"
#include "GameplayUserButtonBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UGameplayUserButtonBase(const FObjectInitializer& ObjectInitializer);

	//~Begin UUserWidget interface
	virtual void NativePreConstruct() override;
	//~End of UUserWidget interface

	//~Begin UCommonButtonBase interface
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~End of UCommonButtonBase interface

	UFUNCTION(BlueprintCallable, Category = "Gameplay User Button")
	void SetButtonText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "Gameplay User Button")
	FText GetButtonText() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay User Button")
	void SetButtonBrush(const FSlateBrush& NewBrush);

	UFUNCTION(BlueprintCallable, Category = "Gameplay User Button")
	FSlateBrush GetButtonBrush() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Highlight")
	bool bToggleHighlightWhenHovered;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Highlight", meta=(EditCondition = "bToggleHighlightWhenHovered"))
	TSubclassOf<UCommonButtonStyle> HighlightButtonStyleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Highlight", meta=(EditCondition = "bToggleHighlightWhenHovered"))
	TSubclassOf<UCommonButtonStyle> UnhighlightButtonStyleClass;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> Text_Button;

	UPROPERTY(BlueprintReadOnly, Category = "Designer", meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<UCommonLazyImage> Image_Button;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess = true))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess = true))
	FText ButtonDescriptionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess = true))
	bool bUseUppercaseForButtonText = false;
};
