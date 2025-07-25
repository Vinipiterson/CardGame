// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/Texture2D.h"
#include "DeveloperSettings_UserInterface.generated.h"

class UGameplayUserActivatableWidget;
class UGameplayUserPolicy;
class UGameplayUserDataRegistry;
class UGameplayUserConfirmationDialogWidget;

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Gameplay User Interface Settings"))
class GAMEPLAYUSERINTERFACE_API UDeveloperSettings_UserInterface : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Policy")
	TSoftClassPtr<UGameplayUserPolicy> GameplayUserPolicyClass;
	
	UPROPERTY(Config, EditAnywhere, Category = "Screen")
	TSoftClassPtr<UGameplayUserConfirmationDialogWidget> ConfirmationDialogWidgetClass;

	UPROPERTY(Config, EditAnywhere, Category = "Screen", meta = (ForceInlineRow, Categories = "GameplayUser.Screen"))
	TMap<FGameplayTag, TSoftClassPtr<UGameplayUserActivatableWidget>> ScreenWidgets;

	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	TSoftClassPtr<UGameplayUserDataRegistry> GameplayUserDataRegistryClass;

	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	bool bShouldApplySettingsImmediately;

	UPROPERTY(Config, EditAnywhere, Category = "Settings", meta = (ForceInlineRow, Categories = "GameplayUser.Description"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> DescriptionImages;

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MasterSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MusicSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass", DisplayName = "Sound FX Sound Class"))
	FSoftObjectPath SoundFXSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Audio", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
	FSoftObjectPath DefaultSoundMix;

public:
	UDeveloperSettings_UserInterface();

	static const UDeveloperSettings_UserInterface* Get();

	TSoftClassPtr<UGameplayUserConfirmationDialogWidget> GetDefaultConfirmationDialogWidget() const;
};
