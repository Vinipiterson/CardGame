// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Foundation/GameplayUserActionWidget.h"
#include "CommonInputSubsystem.h"
#include "CommonUITypes.h"

FSlateBrush UGameplayUserActionWidget::GetIcon() const
{
	const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (!IsValid(CommonInputSubsystem))
	{
		return Super::GetIcon();
	}

	FSlateBrush IconBrush = CommonUI::IsEnhancedInputSupportEnabled() ?
		CommonUI::GetIconForEnhancedInputAction(CommonInputSubsystem, EnhancedInputAction) : CommonUI::GetIconForInputActions(CommonInputSubsystem, InputActions);
				
	if (bOverrideIconSize)
	{
		IconBrush.ImageSize = IconSize;
	}

	return IconBrush;
}
