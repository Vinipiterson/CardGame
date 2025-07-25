// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonRotator.h"
#include "Input/NavigationReply.h"
#include "GameplayUserRotator.generated.h"

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserRotator : public UCommonRotator
{
	GENERATED_BODY()

public:
	void SetSelectedOptionByText(const FText& TextOption);

	//~Begin UCommonRotator interface
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	//~End of UCommonRotator interface

	FNavigationDelegate OnCustomNavigation;

	//~Begin UUserWidget interface
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	//~End of UUserWidget interface

	UFUNCTION(BlueprintCallable, Category="Gameplay User Rotator")
	void SetTextStyle(TSubclassOf<UCommonTextStyle> NewStyle);

protected:
	virtual TSharedPtr<SWidget> NativeOnCustomNavigation(EUINavigation UINavigation);
};
