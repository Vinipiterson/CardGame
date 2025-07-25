// Copyright Spike Plugins 2025. All Rights Reserved.

#include "DeveloperSettings/DeveloperSettings_UserInterface.h"
#include "GameFramework/GameplayUserPolicy.h"
#include "GameFramework/Registry/GameplayUserDataRegistry.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Internationalization/StringTable.h"
#include "Widgets/Dialog/GameplayUserConfirmationDialogWidget.h"

UDeveloperSettings_UserInterface::UDeveloperSettings_UserInterface()
{
	GameplayUserPolicyClass = UGameplayUserPolicy::StaticClass();
	GameplayUserDataRegistryClass = UGameplayUserDataRegistry::StaticClass();

	static ConstructorHelpers::FClassFinder<UGameplayUserConfirmationDialogWidget> ConfirmationDialogWidgetClassFinder(TEXT("/GameplayUserInterface/Blueprints/Widgets/Screens/WBP_GUI_Screen_ConfirmationDialog"));
	if (ConfirmationDialogWidgetClassFinder.Succeeded())
	{
		ConfirmationDialogWidgetClass = ConfirmationDialogWidgetClassFinder.Class;
	}

	bShouldApplySettingsImmediately = false;

	static ConstructorHelpers::FObjectFinder<USoundClass> MasterSoundClassFinder(TEXT("/Engine/EngineSounds/Master"));
	if (MasterSoundClassFinder.Succeeded())
	{
		MasterSoundClass = MasterSoundClassFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> MusicSoundClassFinder(TEXT("/Engine/EngineSounds/Music"));
	if (MusicSoundClassFinder.Succeeded())
	{
		MusicSoundClass = MusicSoundClassFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> SFXSoundClassFinder(TEXT("/Engine/EngineSounds/SFX"));
	if (SFXSoundClassFinder.Succeeded())
	{
		SoundFXSoundClass = SFXSoundClassFinder.Object;
	}
}

const UDeveloperSettings_UserInterface* UDeveloperSettings_UserInterface::Get()
{
	return GetDefault<UDeveloperSettings_UserInterface>();
}

TSoftClassPtr<UGameplayUserConfirmationDialogWidget> UDeveloperSettings_UserInterface::GetDefaultConfirmationDialogWidget() const
{
	return ConfirmationDialogWidgetClass;
}
