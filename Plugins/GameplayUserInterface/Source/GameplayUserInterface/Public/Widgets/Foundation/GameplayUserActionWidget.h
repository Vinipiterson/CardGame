// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonActionWidget.h"
#include "GameplayUserActionWidget.generated.h"

UCLASS(Blueprintable, BlueprintType)
class GAMEPLAYUSERINTERFACE_API UGameplayUserActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()

public:
	//~Begin UCommonActionWidget interface
	virtual FSlateBrush GetIcon() const override;
	//~End of UCommonActionWidget interface

private:
	UPROPERTY(EditAnywhere, Category = "CommonActionWidget")
	bool bOverrideIconSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CommonActionWidget", meta = (EditCondition = bOverrideIconSize, AllowPrivateAccess = true))
	FVector2D IconSize;
};
