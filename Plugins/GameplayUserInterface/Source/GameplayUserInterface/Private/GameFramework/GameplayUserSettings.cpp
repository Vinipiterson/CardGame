// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/GameplayUserSettings.h"
#include "DeveloperSettings/DeveloperSettings_UserInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Engine/Engine.h"

UGameplayUserSettings::UGameplayUserSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, OverallVolume(1.0f)
	, MusicVolume(1.0f)
	, SFXVolume(1.0f)
	, bAllowBackgroundAudio(false)
	, bHDRAudioMode(false)
{
	
}

UGameplayUserSettings* UGameplayUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UGameplayUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

//-------------------------------------------------------
// Audio Collection Tab
//-------------------------------------------------------
void UGameplayUserSettings::SetOverallVolume(float NewOverallVolume)
{
	UWorld* AudioWorld = nullptr;
	const UDeveloperSettings_UserInterface* GameplayUserInterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();

	if (GEngine)
	{
		AudioWorld = GEngine->GetCurrentPlayWorld();
	}

	if (!AudioWorld || !GameplayUserInterfaceSettings)
	{
		return;
	}

	USoundClass* MasterSoundClass = nullptr;
	if (UObject* LoadedObject = GameplayUserInterfaceSettings->MasterSoundClass.TryLoad())
	{
		MasterSoundClass = CastChecked<USoundClass>(LoadedObject);
	}

	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = GameplayUserInterfaceSettings->DefaultSoundMix.TryLoad())
	{
		DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	}
	
	OverallVolume = NewOverallVolume;

	UGameplayStatics::SetSoundMixClassOverride(AudioWorld, DefaultSoundMix, MasterSoundClass, OverallVolume, 1.0f, 0.2f);
	UGameplayStatics::PushSoundMixModifier(AudioWorld, DefaultSoundMix);
}

void UGameplayUserSettings::SetMusicVolume(float NewMusicVolume)
{
	UWorld* AudioWorld = nullptr;
	const UDeveloperSettings_UserInterface* GameplayUserInterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();

	if (GEngine)
	{
		AudioWorld = GEngine->GetCurrentPlayWorld();
	}

	if (!AudioWorld || !GameplayUserInterfaceSettings)
	{
		return;
	}

	USoundClass* MusicSoundClass = nullptr;
	if (UObject* LoadedObject = GameplayUserInterfaceSettings->MusicSoundClass.TryLoad())
	{
		MusicSoundClass = CastChecked<USoundClass>(LoadedObject);
	}

	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = GameplayUserInterfaceSettings->DefaultSoundMix.TryLoad())
	{
		DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	}

	MusicVolume = NewMusicVolume;

	UGameplayStatics::SetSoundMixClassOverride(AudioWorld, DefaultSoundMix, MusicSoundClass, MusicVolume, 1.0f, 0.2f);
	UGameplayStatics::PushSoundMixModifier(AudioWorld, DefaultSoundMix);
}

void UGameplayUserSettings::SetSFXVolume(float NewSFXVolume)
{
	UWorld* AudioWorld = nullptr;
	const UDeveloperSettings_UserInterface* GameplayUserInterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();

	if (GEngine)
	{
		AudioWorld = GEngine->GetCurrentPlayWorld();
	}

	if (!AudioWorld || !GameplayUserInterfaceSettings)
	{
		return;
	}

	USoundClass* SFXSoundClass = nullptr;
	if (UObject* LoadedObject = GameplayUserInterfaceSettings->SoundFXSoundClass.TryLoad())
	{
		SFXSoundClass = CastChecked<USoundClass>(LoadedObject);
	}

	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = GameplayUserInterfaceSettings->DefaultSoundMix.TryLoad())
	{
		DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	}

	SFXVolume = NewSFXVolume;

	UGameplayStatics::SetSoundMixClassOverride(AudioWorld, DefaultSoundMix, SFXSoundClass, SFXVolume, 1.0f, 0.2f);
	UGameplayStatics::PushSoundMixModifier(AudioWorld, DefaultSoundMix);
}

void UGameplayUserSettings::SetIsBackgroundAudioAllowed(bool bNewAllowed)
{
	bAllowBackgroundAudio = bNewAllowed;

	const float BackgroundVolume = bAllowBackgroundAudio ? 1.f : 0.f;

	FApp::SetUnfocusedVolumeMultiplier(BackgroundVolume);
}

void UGameplayUserSettings::SetIsHDRAudioMode(bool bNewAllowed)
{
	bHDRAudioMode = bNewAllowed;
}

//-------------------------------------------------------
// Vide Collection Tab
//-------------------------------------------------------
float UGameplayUserSettings::GetDisplayGamma() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}

	return 0.0f;
}

void UGameplayUserSettings::SetDisplayGamma(float NewGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = NewGamma;
	}
}
