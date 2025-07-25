// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameFramework/GameUserSettings.h"
#include "GameplayUserSettings.generated.h"

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UGameplayUserSettings(const FObjectInitializer& ObjectInitializer);
	
	static UGameplayUserSettings* Get();

	//-------------------------------------------------------
	// Gameplay Collection Tab
	//-------------------------------------------------------
public:
	UFUNCTION()
	FString GetGameDifficulty() const { return GameDifficulty; }

	UFUNCTION()
	void SetGameDifficulty(const FString& NewGameDifficulty) { GameDifficulty = NewGameDifficulty; }

private:
	UPROPERTY(Config)
	FString GameDifficulty;

	//-------------------------------------------------------
	// Audio Collection Tab
	//-------------------------------------------------------
public:
	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; }

	UFUNCTION()
	void SetOverallVolume(float NewOverallVolume);

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetMusicVolume(float NewMusicVolume);

	UFUNCTION()
	float GetSFXVolume() const { return SFXVolume; }

	UFUNCTION()
	void SetSFXVolume(float NewSFXVolume);

	UFUNCTION()
	bool IsBackgroundAudioAllowed() const { return bAllowBackgroundAudio; }

	UFUNCTION()
	void SetIsBackgroundAudioAllowed(bool bNewAllowed);

	UFUNCTION()
	bool IsHDRAudioMode() const { return bHDRAudioMode; }

	UFUNCTION()
	void SetIsHDRAudioMode(bool bNewAllowed);
	
private:
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SFXVolume;

	UPROPERTY(Config)
	bool bAllowBackgroundAudio;
	
	UPROPERTY(Config)
	bool bHDRAudioMode;

	//-------------------------------------------------------
	// Video Collection Tab
	//-------------------------------------------------------
public:
	UFUNCTION()
	float GetDisplayGamma() const;

	UFUNCTION()
	void SetDisplayGamma(float NewGamma);
};
