// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Library/GameplayUserLibrary.h"
#include "DeveloperSettings/DeveloperSettings_UserInterface.h"

TSoftClassPtr<UGameplayUserActivatableWidget> UGameplayUserLibrary::GetGameplayUserScreenByTag(FGameplayTag Tag)
{
	const UDeveloperSettings_UserInterface* Settings = GetDefault<UDeveloperSettings_UserInterface>();
	if (Settings)
	{
		checkf(Settings->ScreenWidgets.Contains(Tag), TEXT("Could not find the corresponding widget under the tag %s"), *Tag.ToString());
		return Settings->ScreenWidgets.FindRef(Tag);
	}

	return nullptr;
}

TSoftObjectPtr<UTexture2D> UGameplayUserLibrary::GetGameplayUserDescriptionImageByTag(FGameplayTag Tag)
{
	const UDeveloperSettings_UserInterface* Settings = GetDefault<UDeveloperSettings_UserInterface>();
	if (Settings)
	{
		checkf(Settings->DescriptionImages.Contains(Tag), TEXT("Could not find the corresponding image under the tag %s"), *Tag.ToString());
		return Settings->DescriptionImages.FindRef(Tag);
	}

	return nullptr;
}

TSoftClassPtr<UGameplayUserConfirmationDialogWidget> UGameplayUserLibrary::GetConfirmationDialogWidget()
{	
	const UDeveloperSettings_UserInterface* Settings = GetDefault<UDeveloperSettings_UserInterface>();
	if (Settings)
	{
		return Settings->GetDefaultConfirmationDialogWidget();
	}

	return nullptr;
}
