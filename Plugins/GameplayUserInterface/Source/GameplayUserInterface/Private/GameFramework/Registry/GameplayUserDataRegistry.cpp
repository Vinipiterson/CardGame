// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/Registry/GameplayUserDataRegistry.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/List/GameplayUserListData_Collection.h"
#include "GameFramework/List/GameplayUserListData_String.h"
#include "GameFramework/GameplayUserSettings.h"
#include "GameFramework/List/GameplayUserListData_Scalar.h"
#include "GameFramework/List/GameplayUserListData_StringResolution.h"
#include "DeveloperSettings/DeveloperSettings_UserInterface.h"
#include "Library/GameplayUserLibrary.h"
#include "GameplayUserOptionsDataAccessor.h"
#include "GameplayUserTags.h"
#include "GameFramework/List/GameplayUserListData_KeyRemap.h"
#include "Internationalization/StringTableRegistry.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#define LOCTEXT_NAMESPACE "GameplayUserDataRegistry"

#define GET_DESCRIPTION(InKey) LOCTABLE("/GameplayUserInterface/Blueprints/Misc/Internationalization/ST_OptionsDescriptions.ST_OptionsDescriptions", InKey)

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserDataRegistry, Log, All);

UGameplayUserDataRegistry::UGameplayUserDataRegistry()
{
}

void UGameplayUserDataRegistry::InitRegistry(ULocalPlayer* LocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab(LocalPlayer);
}

TArray<UGameplayUserListData*> UGameplayUserDataRegistry::GetListSourceItemsBySelectedTabID(const FName& TabID) const
{
	UGameplayUserListData_Collection* const* FoundTabCollectionPtr = RegisteredCollectionTabs.FindByPredicate(
		[TabID](UGameplayUserListData_Collection* AvailableCollection)->bool
		{
			return AvailableCollection->GetDevName() == TabID;
		}
	);

	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID %s"), *TabID.ToString());

	UGameplayUserListData_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UGameplayUserListData*> AllChildListItems;
	for (UGameplayUserListData* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UGameplayUserDataRegistry::FindChildListDataRecursively(UGameplayUserListData* InParentData, TArray<UGameplayUserListData*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UGameplayUserListData* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UGameplayUserDataRegistry::InitGameplayCollectionTab()
{
	const UDeveloperSettings_UserInterface* GameplayUserInterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();
	bool bShouldApplySettingsImmediately = GameplayUserInterfaceSettings->bShouldApplySettingsImmediately;

	UGameplayUserListData_Collection* GameplayCollection = NewObject<UGameplayUserListData_Collection>();
	if (!IsValid(GameplayCollection)) return;

	GameplayCollection->SetDevName(FName("GameplayCollection"));
	GameplayCollection->SetDisplayName(LOCTEXT("GameplayCollection_Name", "Gameplay"));

	//---------------------------------------------
	// Collection: Game
	//---------------------------------------------
	{
		UGameplayUserListData_Collection* GameCollection = NewObject<UGameplayUserListData_Collection>();
		GameCollection->SetDevName(FName("GameCollection"));
		GameCollection->SetDisplayName(LOCTEXT("GameCollection_Name", "Game"));

		GameplayCollection->AddChildListData(GameCollection);
		
		//---------------------------------------------
		// Setting: Game Difficulty
		//---------------------------------------------
		{
			UGameplayUserListData_String* GameDifficulty = NewObject<UGameplayUserListData_String>();

			GameDifficulty->SetDevName(FName("GameDifficulty"));
			GameDifficulty->SetDisplayName(LOCTEXT("GameDifficulty_Name", "Difficulty"));
			GameDifficulty->SetDescriptionText(LOCTEXT("GameDifficulty_Description",
				"Adjust the difficulty of the game experience.\n\n"
				"<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n"
				"<Bold>Normal:</> Offsers slightly harder combat experience.\n\n"
				"<Bold>Hard:</> Offsers a much more challenging combat experience.\n\n"
				"<Bold>Very Hard:</> Offsers a very insane challenging combat experience.\n\n"
				"<Bold>Realistic:</> Offsers a fully realistic combat experience."
			));
		
			GameDifficulty->AddDynamicOption(TEXT("Easy"), LOCTEXT("GameDifficulty_Easy", "Easy"));
			GameDifficulty->AddDynamicOption(TEXT("Normal"), LOCTEXT("GameDifficulty_Normal", "Normal"));
			GameDifficulty->AddDynamicOption(TEXT("Hard"), LOCTEXT("GameDifficulty_Hard", "Hard"));
			GameDifficulty->AddDynamicOption(TEXT("Very Hard"), LOCTEXT("GameDifficulty_VeryHard", "Very Hard"));
			GameDifficulty->AddDynamicOption(TEXT("Realistic"), LOCTEXT("GameDifficulty_Realistic", "Realistic"));

			GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));

			GameDifficulty->SetDynamicGetter(OPTIONS_ACCESSOR(GetGameDifficulty));
			GameDifficulty->SetDynamicSetter(OPTIONS_ACCESSOR(SetGameDifficulty));

			GameDifficulty->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			GameplayCollection->AddChildListData(GameDifficulty);
		}

		//---------------------------------------------
		// Setting: Simple Test
		//---------------------------------------------
		{
			UGameplayUserListData_String* TestCollection = NewObject<UGameplayUserListData_String>();

			TestCollection->SetDevName(FName("TestCollection"));
			TestCollection->SetDisplayName(LOCTEXT("TestCollection_Name", "Test Setting"));
			TestCollection->SetDescriptionImage(UGameplayUserLibrary::GetGameplayUserDescriptionImageByTag(GameplayUserTags::Description::Description_TestImage));
			TestCollection->SetDescriptionText(LOCTEXT("TestCollection_Description", "The image to display can be specified in the project settings. It can be anything the developer assigned in there"));
		
			GameplayCollection->AddChildListData(TestCollection);
		}
	}
	
	RegisteredCollectionTabs.Add(GameplayCollection);
}

void UGameplayUserDataRegistry::InitAudioCollectionTab()
{
	const UDeveloperSettings_UserInterface* GameplayUserInterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();
	bool bShouldApplySettingsImmediately = GameplayUserInterfaceSettings->bShouldApplySettingsImmediately;

	UGameplayUserListData_Collection* AudioCollection = NewObject<UGameplayUserListData_Collection>();
	if (!IsValid(AudioCollection)) return;
	
	AudioCollection->SetDevName(FName("AudioCollection"));
	AudioCollection->SetDisplayName(LOCTEXT("AudioCollection_Name", "Audio"));

	//---------------------------------------------
	// Collection: Volume
	//---------------------------------------------
	{
		UGameplayUserListData_Collection* VolumeCollection = NewObject<UGameplayUserListData_Collection>();
		VolumeCollection->SetDevName(FName("VolumeCollection"));
		VolumeCollection->SetDisplayName(LOCTEXT("VolumeCollection_Name", "Volume"));

		AudioCollection->AddChildListData(VolumeCollection);

		//---------------------------------------------
		// Setting: Overall Volume
		//---------------------------------------------
		{
			UGameplayUserListData_Scalar* OverallVolume = NewObject<UGameplayUserListData_Scalar>();
			OverallVolume->SetDevName(FName("OverallVolume"));
			OverallVolume->SetDisplayName(LOCTEXT("OverallVolume_Name", "Overall Volume"));
			OverallVolume->SetDescriptionText(LOCTEXT("OverallVolume_Description", "This is Description for Overall Volume setting"));
			
			OverallVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.0f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UGameplayUserListData_Scalar::NoDecimalFormattingOptions()); // No Decimal: 50% // One Decimal: 50.5%

			OverallVolume->SetDynamicGetter(OPTIONS_ACCESSOR(GetOverallVolume));
			OverallVolume->SetDynamicSetter(OPTIONS_ACCESSOR(SetOverallVolume));

			OverallVolume->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			VolumeCollection->AddChildListData(OverallVolume);
		}
		
		//---------------------------------------------
		// Setting: Music Volume
		//---------------------------------------------
		{
			UGameplayUserListData_Scalar* MusicVolume = NewObject<UGameplayUserListData_Scalar>();
			MusicVolume->SetDevName(FName("MusicVolume"));
			MusicVolume->SetDisplayName(LOCTEXT("MusicVolume_Name", "Music Volume"));
			MusicVolume->SetDescriptionText(LOCTEXT("MusicVolume_Description", "This is Description for Music Volume setting"));

			MusicVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.0f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UGameplayUserListData_Scalar::NoDecimalFormattingOptions()); // No Decimal: 50% // One Decimal: 50.5%

			MusicVolume->SetDynamicGetter(OPTIONS_ACCESSOR(GetMusicVolume));
			MusicVolume->SetDynamicSetter(OPTIONS_ACCESSOR(SetMusicVolume));

			MusicVolume->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			VolumeCollection->AddChildListData(MusicVolume);
		}

		//---------------------------------------------
		// Setting: Sound FX Volume
		//---------------------------------------------
		{
			UGameplayUserListData_Scalar* SFXVolume = NewObject<UGameplayUserListData_Scalar>();
			SFXVolume->SetDevName(FName("SFXVolume"));
			SFXVolume->SetDisplayName(LOCTEXT("SFXVolume_Name", "SFX Volume"));
			SFXVolume->SetDescriptionText(LOCTEXT("SFXVolume_Description", "This is Description for SFX Volume setting"));
			
			SFXVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			SFXVolume->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			SFXVolume->SetSliderStepSize(0.01f);
			SFXVolume->SetDefaultValueFromString(LexToString(1.0f));
			SFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SFXVolume->SetNumberFormattingOptions(UGameplayUserListData_Scalar::NoDecimalFormattingOptions()); // No Decimal: 50% // One Decimal: 50.5%

			SFXVolume->SetDynamicGetter(OPTIONS_ACCESSOR(GetSFXVolume));
			SFXVolume->SetDynamicSetter(OPTIONS_ACCESSOR(SetSFXVolume));

			SFXVolume->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			VolumeCollection->AddChildListData(SFXVolume);
		}
	}

	//---------------------------------------------
	// Collection: Sound
	//---------------------------------------------
	{
		UGameplayUserListData_Collection* SoundCollection = NewObject<UGameplayUserListData_Collection>();
		SoundCollection->SetDevName(FName("SoundCollection"));
		SoundCollection->SetDisplayName(LOCTEXT("SoundCollection_Name", "Sound"));

		AudioCollection->AddChildListData(SoundCollection);

		//---------------------------------------------
		// Setting: Allow Background Audio
		//---------------------------------------------
		{
			UGameplayUserListData_StringBool* AllowBackgroundAudio = NewObject<UGameplayUserListData_StringBool>();
			AllowBackgroundAudio->SetDevName(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDisplayName(LOCTEXT("AllowBackgroundAudio_Name", "Allow Background Audio"));
			AllowBackgroundAudio->SetDescriptionText(LOCTEXT("AllowBackgroundAudio_Description", "This is Description for Allow Background Audio setting."));

			AllowBackgroundAudio->OverrideFalseDisplayText(LOCTEXT("AllowBackgroundAudio_FalseName", "Disabled"));
			AllowBackgroundAudio->OverrideTrueDisplayText(LOCTEXT("AllowBackgroundAudio_TrueName", "Enabled"));

			AllowBackgroundAudio->SetFalseAsDefaultValue();
			
			AllowBackgroundAudio->SetDynamicGetter(OPTIONS_ACCESSOR(IsBackgroundAudioAllowed));
			AllowBackgroundAudio->SetDynamicSetter(OPTIONS_ACCESSOR(SetIsBackgroundAudioAllowed));

			AllowBackgroundAudio->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			SoundCollection->AddChildListData(AllowBackgroundAudio);
		}

		//---------------------------------------------
		// Setting: HDR Audio
		//---------------------------------------------
		{
			UGameplayUserListData_StringBool* HDRMode = NewObject<UGameplayUserListData_StringBool>();
			HDRMode->SetDevName(FName("HDRMode"));
			HDRMode->SetDisplayName(LOCTEXT("HDRMode_Name", "HDR Audio"));
			HDRMode->SetDescriptionText(LOCTEXT("HDRMode_Description", "This is Description for HDR Audio setting."));

			HDRMode->OverrideFalseDisplayText(LOCTEXT("HDRMode_FalseName", "Disabled"));
			HDRMode->OverrideTrueDisplayText(LOCTEXT("HDRMode_TrueName", "Enabled"));

			HDRMode->SetFalseAsDefaultValue();
			
			HDRMode->SetDynamicGetter(OPTIONS_ACCESSOR(IsHDRAudioMode));
			HDRMode->SetDynamicSetter(OPTIONS_ACCESSOR(SetIsHDRAudioMode));

			HDRMode->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			SoundCollection->AddChildListData(HDRMode);
		}
	}

	RegisteredCollectionTabs.Add(AudioCollection);
}

void UGameplayUserDataRegistry::InitVideoCollectionTab()
{
	const UDeveloperSettings_UserInterface* GameplayUserInterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();
	bool bShouldApplySettingsImmediately = GameplayUserInterfaceSettings->bShouldApplySettingsImmediately;
	
	UGameplayUserListData_Collection* VideoCollection = NewObject<UGameplayUserListData_Collection>();
	VideoCollection->SetDevName(FName("VideoCollection"));
	VideoCollection->SetDisplayName(LOCTEXT("VideoCollection_Name", "Video"));

	UGameplayUserListData_StringEnum* WindowModeDependency = nullptr;
	UGameplayUserListData_StringInteger* OverallQualityDependency = nullptr;

	//---------------------------------------------
	// Collection: Display
	//---------------------------------------------
	{
		UGameplayUserListData_Collection* DisplayCollection = NewObject<UGameplayUserListData_Collection>();
		DisplayCollection->SetDevName(FName("DisplayCollection"));
		DisplayCollection->SetDisplayName(LOCTEXT("DisplayCollection_Name", "Display"));

		VideoCollection->AddChildListData(DisplayCollection);

		FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc([]()->bool
		{
			const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;
			return !bIsInEditor;
		});
		FText PackagedBuildRichText = LOCTEXT("PackagedBuildOnly_DisabledReason", "\n\n<Disabled>This setting can only be adjust in a packaged build.</>");
		PackagedBuildOnlyCondition.SetDisableRichReason(PackagedBuildRichText.ToString());

		//---------------------------------------------
		// Setting: Window Mode
		//---------------------------------------------
		{
			UGameplayUserListData_StringEnum* WindowMode = NewObject<UGameplayUserListData_StringEnum>();
			WindowMode->SetDevName(FName("WindowMode"));
			WindowMode->SetDisplayName(LOCTEXT("WindowMode_Name", "Window Mode"));
			WindowMode->SetDescriptionText(LOCTEXT("WindowMode_Description", 
				"<Bold>Windowed mode:</> you can interact with other windows more easily, and drag the edges of the window to set the size.\n\n"
				"<Bold>Borderless Window:</> you can easily switch between applications.\n\n"
				"<Bold>Fullscreen Mode:</> you cannot interact with other windows as easily, but the game will run slightly faster."
			));

			WindowMode->AddEnumOption(EWindowMode::Windowed, LOCTEXT("WindowMode_WindowedName", "Windowed"));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, LOCTEXT("WindowMode_WindowedFullscreenName", "Windowed Fullscreen"));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, LOCTEXT("WindowMode_FullscreenName", "Fullscreen"));

			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);

			WindowMode->SetDynamicGetter(OPTIONS_ACCESSOR(GetFullscreenMode));
			WindowMode->SetDynamicSetter(OPTIONS_ACCESSOR(SetFullscreenMode));

			WindowMode->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			WindowModeDependency = WindowMode;

			DisplayCollection->AddChildListData(WindowMode);
		}
		
		//---------------------------------------------
		// Setting: Screen Resolution
		//---------------------------------------------
		{
			UGameplayUserListData_StringResolution* ScreenResolution = NewObject<UGameplayUserListData_StringResolution>();
			ScreenResolution->SetDevName(FName("ScreenResolution"));
			ScreenResolution->SetDisplayName(LOCTEXT("ScreenResolution_Name", "Screen Resolution"));
			ScreenResolution->SetDescriptionText(LOCTEXT("ScreenResolution_Description", 
				"Display Resolution determines the size of the window in Windowed mode."
				"In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card."
				"Display Resolution is inactive in Windowed Fullscreen mode."
			));

			ScreenResolution->InitResolutionValues();

			ScreenResolution->SetDynamicGetter(OPTIONS_ACCESSOR(GetScreenResolution));
			ScreenResolution->SetDynamicSetter(OPTIONS_ACCESSOR(SetScreenResolution));

			ScreenResolution->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			FOptionsDataEditConditionDescriptor WindowModeCondition;
			WindowModeCondition.SetEditConditionFunc([WindowModeDependency]()->bool
			{
				const bool bIsBorderlessWindow = WindowModeDependency->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;
				return !bIsBorderlessWindow;
			});
			FText WindowModeRichText = LOCTEXT("WindowModeCondition_DisabledReason",
				"\n\n<Disabled>Screen Resolution is not adjustable when the 'Window Mode' is set to 'Windowed Fullscreen'. "
				"The value must match with the maximum allowed resolution.</>"
			);
			WindowModeCondition.SetDisableRichReason(WindowModeRichText.ToString());
			WindowModeCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditDependency(WindowModeDependency);
			
			ScreenResolution->AddEditCondition(WindowModeCondition);
			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			DisplayCollection->AddChildListData(ScreenResolution);
		}
	}

	//---------------------------------------------
	// Collection: Graphics
	//---------------------------------------------
	{
		UGameplayUserListData_Collection* GraphicsCollection = NewObject<UGameplayUserListData_Collection>();
		GraphicsCollection->SetDevName(FName("GraphicsCollection"));
		GraphicsCollection->SetDisplayName(LOCTEXT("GraphicsCollection_Name", "Graphics"));

		VideoCollection->AddChildListData(GraphicsCollection);

		//---------------------------------------------
		// Setting: Overall Quality
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* OverallQuality = NewObject<UGameplayUserListData_StringInteger>();
			OverallQuality->SetDevName(FName("OverallQuality"));
			OverallQuality->SetDisplayName(LOCTEXT("OverallQuality_Name", "Overall Quality"));
			OverallQuality->SetDescriptionText(LOCTEXT("OverallQuality_Description", 
				"Quality Preset allows you to adjust multiple video options at once."
				"Try a few options to see what fits your preference and device's performance."
			));

			OverallQuality->AddIntegerOption(0, LOCTEXT("OverallQuality_Low", "Low"));
			OverallQuality->AddIntegerOption(1, LOCTEXT("OverallQuality_Medium", "Medium"));
			OverallQuality->AddIntegerOption(2, LOCTEXT("OverallQuality_High", "High"));
			OverallQuality->AddIntegerOption(3, LOCTEXT("OverallQuality_Epic", "Epic"));
			OverallQuality->AddIntegerOption(4, LOCTEXT("OverallQuality_Cinematic", "Cinematic"));
			
			OverallQuality->SetDynamicGetter(OPTIONS_ACCESSOR(GetOverallScalabilityLevel));
			OverallQuality->SetDynamicSetter(OPTIONS_ACCESSOR(SetOverallScalabilityLevel));

			OverallQuality->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			GraphicsCollection->AddChildListData(OverallQuality);

			OverallQualityDependency = OverallQuality;
		}

		//---------------------------------------------
		// Setting: Display Gamma
		//---------------------------------------------
		{
			UGameplayUserListData_Scalar* DisplayGamma = NewObject<UGameplayUserListData_Scalar>();
			DisplayGamma->SetDevName(FName("DisplayGamma"));
			DisplayGamma->SetDisplayName(LOCTEXT("DisplayGamma_Name", "Display Gamma"));
			DisplayGamma->SetDescriptionText(LOCTEXT("DisplayGamma_Description", "Adjusts the brightness."));
			
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); // The default value Unreal has is: 2.2f.
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UGameplayUserListData_Scalar::NoDecimalFormattingOptions());
			DisplayGamma->SetSliderStepSize(0.01f);

			DisplayGamma->SetDynamicGetter(OPTIONS_ACCESSOR(GetDisplayGamma));
			DisplayGamma->SetDynamicSetter(OPTIONS_ACCESSOR(SetDisplayGamma));

			GraphicsCollection->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

			GraphicsCollection->AddChildListData(DisplayGamma);
		}

		//---------------------------------------------
		// Setting: 3D Resolution Scale
		//---------------------------------------------
		{
			UGameplayUserListData_Scalar* ResolutionScale = NewObject<UGameplayUserListData_Scalar>();
			ResolutionScale->SetDevName(FName("ResolutionScale"));
			ResolutionScale->SetDisplayName(LOCTEXT("ResolutionScale_Name", "3D Resolution"));
			ResolutionScale->SetDescriptionText(LOCTEXT("ResolutionScale_Description", 
				"3D resolution determines the resolution that objects are rendered in game, but does not affect the main menu." 
				"Lower resolutions can significantly increase frame rate."
			));

			ResolutionScale->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UGameplayUserListData_Scalar::NoDecimalFormattingOptions());
			ResolutionScale->SetSliderStepSize(0.01f);

			ResolutionScale->SetDynamicGetter(OPTIONS_ACCESSOR(GetResolutionScaleNormalized));
			ResolutionScale->SetDynamicSetter(OPTIONS_ACCESSOR(SetResolutionScaleNormalized));

			ResolutionScale->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			ResolutionScale->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(ResolutionScale);

			GraphicsCollection->AddChildListData(ResolutionScale);
		}

		//---------------------------------------------
		// Setting: Global Illumination
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* GlobalIllumination = NewObject<UGameplayUserListData_StringInteger>();
			GlobalIllumination->SetDevName(FName("GlobalIllumination"));
			GlobalIllumination->SetDisplayName(LOCTEXT("GlobalIllumination_Name", "Global Illumination"));
			GlobalIllumination->SetDescriptionText(LOCTEXT("GlobalIllumination_Description", 
				"Global Illumination controls the quality of dynamically calculated indirect lighting bounces, sky shadowing and Ambient Occlusion."
				"Settings of 'High' and above use more accurate ray tracing methods to solve lighting, but can reduce performance."
			));

			GlobalIllumination->AddIntegerOption(0, LOCTEXT("GlobalIllumination_Low", "Low"));
			GlobalIllumination->AddIntegerOption(1, LOCTEXT("GlobalIllumination_Medium", "Medium"));
			GlobalIllumination->AddIntegerOption(2, LOCTEXT("GlobalIllumination_High", "High"));
			GlobalIllumination->AddIntegerOption(3, LOCTEXT("GlobalIllumination_Epic", "Epic"));
			GlobalIllumination->AddIntegerOption(4, LOCTEXT("GlobalIllumination_Cinematic", "Cinematic"));
			
			GlobalIllumination->SetDynamicGetter(OPTIONS_ACCESSOR(GetGlobalIlluminationQuality));
			GlobalIllumination->SetDynamicSetter(OPTIONS_ACCESSOR(SetGlobalIlluminationQuality));

			GlobalIllumination->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			GlobalIllumination->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(GlobalIllumination);
			
			GraphicsCollection->AddChildListData(GlobalIllumination);
		}

		//---------------------------------------------
		// Setting: Shadow Quality
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* ShadowQuality = NewObject<UGameplayUserListData_StringInteger>();
			ShadowQuality->SetDevName(FName("ShadowQuality"));
			ShadowQuality->SetDisplayName(LOCTEXT("ShadowQuality_Name", "Shadow Quality"));
			ShadowQuality->SetDescriptionText(LOCTEXT("ShadowQuality_Description", 
				"Shadow quality determines the resolution and view distance of dynamic shadows." 
				"Shadows improve visual quality and give better depth perception, but can reduce performance."
			));

			ShadowQuality->AddIntegerOption(0, LOCTEXT("ShadowQuality_Low", "Low"));
			ShadowQuality->AddIntegerOption(1, LOCTEXT("ShadowQuality_Medium", "Medium"));
			ShadowQuality->AddIntegerOption(2, LOCTEXT("ShadowQuality_High", "High"));
			ShadowQuality->AddIntegerOption(3, LOCTEXT("ShadowQuality_Epic", "Epic"));
			ShadowQuality->AddIntegerOption(4, LOCTEXT("ShadowQuality_Cinematic", "Cinematic"));
			
			ShadowQuality->SetDynamicGetter(OPTIONS_ACCESSOR(GetShadowQuality));
			ShadowQuality->SetDynamicSetter(OPTIONS_ACCESSOR(SetShadowQuality));

			ShadowQuality->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			ShadowQuality->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(ShadowQuality);
			
			GraphicsCollection->AddChildListData(ShadowQuality);
		}

		//---------------------------------------------
		// Setting: Anti-Aliasing Quality
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* AntiAliasingQuality = NewObject<UGameplayUserListData_StringInteger>();
			AntiAliasingQuality->SetDevName(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDisplayName(LOCTEXT("AntiAliasingQuality_Name", "Anti Aliasing Quality"));
			AntiAliasingQuality->SetDescriptionText(LOCTEXT("AntiAliasingQuality_Description", 
				"Anti-Aliasing reduces jaggy artifacts along geometry edges." 
				"Increasing this setting will make edges look smoother, but can reduce performance."
				"Higher settings mean more anti - aliasing."
			));

			AntiAliasingQuality->AddIntegerOption(0, LOCTEXT("AntiAliasingQuality_Low", "Low"));
			AntiAliasingQuality->AddIntegerOption(1, LOCTEXT("AntiAliasingQuality_Medium", "Medium"));
			AntiAliasingQuality->AddIntegerOption(2, LOCTEXT("AntiAliasingQuality_High", "High"));
			AntiAliasingQuality->AddIntegerOption(3, LOCTEXT("AntiAliasingQuality_Epic", "Epic"));
			AntiAliasingQuality->AddIntegerOption(4, LOCTEXT("AntiAliasingQuality_Cinematic", "Cinematic"));
			
			AntiAliasingQuality->SetDynamicGetter(OPTIONS_ACCESSOR(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDynamicSetter(OPTIONS_ACCESSOR(SetAntiAliasingQuality));

			AntiAliasingQuality->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			AntiAliasingQuality->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(AntiAliasingQuality);
			
			GraphicsCollection->AddChildListData(AntiAliasingQuality);
		}

		//---------------------------------------------
		// Setting: View Distance
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* ViewDistanceQuality = NewObject<UGameplayUserListData_StringInteger>();
			ViewDistanceQuality->SetDevName(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDisplayName(LOCTEXT("ViewDistanceQuality_Name", "View Distance Quality"));
			ViewDistanceQuality->SetDescriptionText(LOCTEXT("ViewDistanceQuality_Description", 
				"View distance determines how far away objects are culled for performance."
			));

			ViewDistanceQuality->AddIntegerOption(0, LOCTEXT("ViewDistanceQuality_Near", "Near"));
			ViewDistanceQuality->AddIntegerOption(1, LOCTEXT("ViewDistanceQuality_Medium", "Medium"));
			ViewDistanceQuality->AddIntegerOption(2, LOCTEXT("ViewDistanceQuality_Far", "Far"));
			ViewDistanceQuality->AddIntegerOption(3, LOCTEXT("ViewDistanceQuality_VeryFar", "Very Far"));
			ViewDistanceQuality->AddIntegerOption(4, LOCTEXT("ViewDistanceQuality_Cinematic", "Cinematic"));
			
			ViewDistanceQuality->SetDynamicGetter(OPTIONS_ACCESSOR(GetViewDistanceQuality));
			ViewDistanceQuality->SetDynamicSetter(OPTIONS_ACCESSOR(SetViewDistanceQuality));

			ViewDistanceQuality->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			ViewDistanceQuality->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(ViewDistanceQuality);
			
			GraphicsCollection->AddChildListData(ViewDistanceQuality);
		}

		//---------------------------------------------
		// Setting: Texture Quality
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* TextureQuality = NewObject<UGameplayUserListData_StringInteger>();
			TextureQuality->SetDevName(FName("TextureQuality"));
			TextureQuality->SetDisplayName(LOCTEXT("TextureQuality_Name", "Texture Quality"));
			TextureQuality->SetDescriptionText(LOCTEXT("TextureQuality_Description", 
				"Texture quality determines the resolution of textures in game." 
				"Increasing this setting will make objects more detailed, but can reduce performance."
			));

			TextureQuality->AddIntegerOption(0, LOCTEXT("TextureQuality_Low", "Low"));
			TextureQuality->AddIntegerOption(1, LOCTEXT("TextureQuality_Medium", "Medium"));
			TextureQuality->AddIntegerOption(2, LOCTEXT("TextureQuality_High", "High"));
			TextureQuality->AddIntegerOption(3, LOCTEXT("TextureQuality_Epic", "Epic"));
			TextureQuality->AddIntegerOption(4, LOCTEXT("TextureQuality_Cinematic", "Cinematic"));
			
			TextureQuality->SetDynamicGetter(OPTIONS_ACCESSOR(GetTextureQuality));
			TextureQuality->SetDynamicSetter(OPTIONS_ACCESSOR(SetTextureQuality));

			TextureQuality->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			TextureQuality->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(TextureQuality);
			
			GraphicsCollection->AddChildListData(TextureQuality);
		}

		//---------------------------------------------
		// Setting: Visual Effects Quality
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* VisualEffectsQuality = NewObject<UGameplayUserListData_StringInteger>();
			VisualEffectsQuality->SetDevName(FName("VisualEffectsQuality"));
			VisualEffectsQuality->SetDisplayName(LOCTEXT("VisualEffectsQuality_Name", "Visual Effects Quality"));
			VisualEffectsQuality->SetDescriptionText(LOCTEXT("VisualEffectQuality_Description", 
				"Effects determines the quality of visual effects and lighting in game." 
				"Increasing this setting will increase the quality of visual effects, but can reduce performance."
			));

			VisualEffectsQuality->AddIntegerOption(0, LOCTEXT("VisualEffectsQuality_Low", "Low"));
			VisualEffectsQuality->AddIntegerOption(1, LOCTEXT("VisualEffectsQuality_Medium", "Medium"));
			VisualEffectsQuality->AddIntegerOption(2, LOCTEXT("VisualEffectsQuality_High", "High"));
			VisualEffectsQuality->AddIntegerOption(3, LOCTEXT("VisualEffectsQuality_Epic", "Epic"));
			VisualEffectsQuality->AddIntegerOption(4, LOCTEXT("VisualEffectsQuality_Cinematic", "Cinematic"));
			
			VisualEffectsQuality->SetDynamicGetter(OPTIONS_ACCESSOR(GetVisualEffectQuality));
			VisualEffectsQuality->SetDynamicSetter(OPTIONS_ACCESSOR(SetVisualEffectQuality));

			VisualEffectsQuality->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			VisualEffectsQuality->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(VisualEffectsQuality);
			
			GraphicsCollection->AddChildListData(VisualEffectsQuality);
		}

		//---------------------------------------------
		// Setting: Reflection Quality
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* ReflectionQuality = NewObject<UGameplayUserListData_StringInteger>();
			ReflectionQuality->SetDevName(FName("ReflectionQuality"));
			ReflectionQuality->SetDisplayName(LOCTEXT("ReflectionQuality_Name", "Reflection Quality"));
			ReflectionQuality->SetDescriptionText(LOCTEXT("ReflectionQuality_Description", 
				"Reflection quality determines the resolution and accuracy of reflections."  
				"Settings of 'High' and above use more accurate ray tracing methods to solve reflections, but can reduce performance."
			));

			ReflectionQuality->AddIntegerOption(0, LOCTEXT("ReflectionQuality_Low", "Low"));
			ReflectionQuality->AddIntegerOption(1, LOCTEXT("ReflectionQuality_Medium", "Medium"));
			ReflectionQuality->AddIntegerOption(2, LOCTEXT("ReflectionQuality_High", "High"));
			ReflectionQuality->AddIntegerOption(3, LOCTEXT("ReflectionQuality_Epic", "Epic"));
			ReflectionQuality->AddIntegerOption(4, LOCTEXT("ReflectionQuality_Cinematic", "Cinematic"));
			
			ReflectionQuality->SetDynamicGetter(OPTIONS_ACCESSOR(GetReflectionQuality));
			ReflectionQuality->SetDynamicSetter(OPTIONS_ACCESSOR(SetReflectionQuality));

			ReflectionQuality->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			ReflectionQuality->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(ReflectionQuality);
			
			GraphicsCollection->AddChildListData(ReflectionQuality);
		}
		
		//---------------------------------------------
		// Setting: Post Processing Quality
		//---------------------------------------------
		{
			UGameplayUserListData_StringInteger* PostProcessingQuality = NewObject<UGameplayUserListData_StringInteger>();
			PostProcessingQuality->SetDevName(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDisplayName(LOCTEXT("PostProcessingQuality_Name", "Post Processing Quality"));
			PostProcessingQuality->SetDescriptionText(LOCTEXT("PostProcessingQuality_Description", 
				"Post Processing effects include Motion Blur, Depth of Field and Bloom." 
				"Increasing this setting improves the quality of post process effects, but can reduce performance."
			));

			PostProcessingQuality->AddIntegerOption(0, LOCTEXT("PostProcessingQuality_Low", "Low"));
			PostProcessingQuality->AddIntegerOption(1, LOCTEXT("PostProcessingQuality_Medium", "Medium"));
			PostProcessingQuality->AddIntegerOption(2, LOCTEXT("PostProcessingQuality_High", "High"));
			PostProcessingQuality->AddIntegerOption(3, LOCTEXT("PostProcessingQuality_Epic", "Epic"));
			PostProcessingQuality->AddIntegerOption(4, LOCTEXT("PostProcessingQuality_Cinematic", "Cinematic"));
			
			PostProcessingQuality->SetDynamicGetter(OPTIONS_ACCESSOR(GetPostProcessingQuality));
			PostProcessingQuality->SetDynamicSetter(OPTIONS_ACCESSOR(SetPostProcessingQuality));

			PostProcessingQuality->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);
			
			PostProcessingQuality->AddEditDependency(OverallQualityDependency);
			OverallQualityDependency->AddEditDependency(PostProcessingQuality);
			
			GraphicsCollection->AddChildListData(PostProcessingQuality);
		}
	}

	//---------------------------------------------
	// Collection: Advanced Graphics
	//---------------------------------------------
	{
		UGameplayUserListData_Collection* AdvancedGraphicsCollection = NewObject<UGameplayUserListData_Collection>();
		AdvancedGraphicsCollection->SetDevName(FName("AdvancedGraphicsCollection"));
		AdvancedGraphicsCollection->SetDisplayName(LOCTEXT("AdvancedGraphicsCollection_Name", "Advanced Graphics"));

		VideoCollection->AddChildListData(AdvancedGraphicsCollection);

		//---------------------------------------------
		// Setting: Vertical Sync (VSync)
		//---------------------------------------------
		{
			UGameplayUserListData_StringBool* VerticalSync = NewObject<UGameplayUserListData_StringBool>();
			VerticalSync->SetDevName(FName("VerticalSync"));
			VerticalSync->SetDisplayName(LOCTEXT("VerticalSync_Name", "Vertical Sync"));
			VerticalSync->SetDescriptionText(LOCTEXT("VerticalSync_Description", 
				"Enabling Vertical Sync eliminates screen tearing by always rendering and presenting a full frame." 
				"Disabling Vertical Sync can give higher frame rate and better input response, but can result in horizontal screen tearing."
			));

			VerticalSync->SetDynamicGetter(OPTIONS_ACCESSOR(IsVSyncEnabled));
			VerticalSync->SetDynamicSetter(OPTIONS_ACCESSOR(SetVSyncEnabled));

			VerticalSync->SetFalseAsDefaultValue();

			VerticalSync->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			FOptionsDataEditConditionDescriptor FullscreenOnlyCondition;
			FullscreenOnlyCondition.SetEditConditionFunc([WindowModeDependency]()->bool
			{
				return WindowModeDependency->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
			});
			const FText FullscreenOnlyConditionDisabledReason = LOCTEXT("FullscreenOnlyCondition_DisabledReason", "\n\n<Disabled>This feature only works if the 'Window Mode' is set to 'Fullscreen'</>");
			FullscreenOnlyCondition.SetDisableRichReason(FullscreenOnlyConditionDisabledReason.ToString());
			const FText FullscreenOnlyConditionForcedValue = LOCTEXT("FullscreenOnlyCondition_ForcedValue", "false");
			FullscreenOnlyCondition.SetDisabledForcedStringValue(FullscreenOnlyConditionForcedValue.ToString());
			
			VerticalSync->AddEditCondition(FullscreenOnlyCondition);
			
			AdvancedGraphicsCollection->AddChildListData(VerticalSync);
		}

		//---------------------------------------------
		// Setting: Frame Rate Limit
		//---------------------------------------------
		{
			UGameplayUserListData_String* FrameRateLimit = NewObject<UGameplayUserListData_String>();
			FrameRateLimit->SetDevName(FName("FrameRateLimit"));
			FrameRateLimit->SetDisplayName(LOCTEXT("FrameRateLimit_Name", "Frame Rate Limit"));
			FrameRateLimit->SetDescriptionText(LOCTEXT("FrameRateLimit_Description", 
				"Frame rate limit sets the highest frame rate that is allowed." 
				"Set this lower for a more consistent frame rate or higher for the best experience on faster machines." 
				"You may need to disable Vsync to reach high frame rates."
			));

			FrameRateLimit->AddDynamicOption(LexToString(30.0f), FText::FromString("30 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(60.0f), FText::FromString("60 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(90.0f), FText::FromString("90 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(120.0f), FText::FromString("120 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(144.0f), FText::FromString("144 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(165.0f), FText::FromString("165 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(200.0f), FText::FromString("200 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(240.0f), FText::FromString("240 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(360.0f), FText::FromString("360 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(540.0f), FText::FromString("540 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(0.0f), FText::FromString("Unlimited"));

			FrameRateLimit->SetDefaultValueFromString(LexToString(0.0f));

			FrameRateLimit->SetDynamicGetter(OPTIONS_ACCESSOR(GetFrameRateLimit));
			FrameRateLimit->SetDynamicSetter(OPTIONS_ACCESSOR(SetFrameRateLimit));

			FrameRateLimit->SetShouldApplySettingImmediately(bShouldApplySettingsImmediately);

			AdvancedGraphicsCollection->AddChildListData(FrameRateLimit);
		}
	}

	RegisteredCollectionTabs.Add(VideoCollection);
}

void UGameplayUserDataRegistry::InitControlCollectionTab(ULocalPlayer* LocalPlayer)
{
	const UDeveloperSettings_UserInterface* GameplayUserInterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();
	bool bShouldApplySettingsImmediately = GameplayUserInterfaceSettings->bShouldApplySettingsImmediately;
	
	UGameplayUserListData_Collection* ControlCollection = NewObject<UGameplayUserListData_Collection>();
	ControlCollection->SetDevName(FName("ControlCollection"));
	ControlCollection->SetDisplayName(LOCTEXT("ControlCollection_Name", "Control"));

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);

	UEnhancedInputUserSettings* InputUserSettings = InputSubsystem->GetUserSettings();
	checkf(InputUserSettings, TEXT("Please, turn on UserSettings on Project Settings."));

	//---------------------------------------------
	// Collection: Keyboard and Mouse
	//---------------------------------------------
	{
		UGameplayUserListData_Collection* KeyboardMouseMapping = NewObject<UGameplayUserListData_Collection>();
		KeyboardMouseMapping->SetDevName(FName("KeyboardMouseMapping"));
		KeyboardMouseMapping->SetDisplayName(LOCTEXT("KeyboardMouseMapping_Name", "Keyboard & Mouse"));

		ControlCollection->AddChildListData(KeyboardMouseMapping);

		//---------------------------------------------
		// Setting: Keyboard and Mouse Inputs
		//---------------------------------------------
		{
			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::S;
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;
			
			//FPlayerMappableKeyQueryOptions GamepadOnly;
			//GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
			//GamepadOnly.bMatchBasicKeyTypes = true;
			
			for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : InputUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyboardMouseOnly))
						{
							UGameplayUserListData_KeyRemap* KeyRemap = NewObject<UGameplayUserListData_KeyRemap>();
							KeyRemap->SetDevName(KeyMapping.GetMappingName());
							KeyRemap->SetDisplayName(FText::Format(LOCTEXT("KeyRemap_Name", "{0}"), KeyMapping.GetDisplayName()));
							KeyRemap->InitKeyRemapData(InputUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMapping);

							KeyboardMouseMapping->AddChildListData(KeyRemap);
						}
					}
				}
			}
		}
	}

	//---------------------------------------------
	// Collection: Keyboard and Mouse
	//---------------------------------------------
	{
		UGameplayUserListData_Collection* GamepadMapping = NewObject<UGameplayUserListData_Collection>();
		GamepadMapping->SetDevName(FName("GamepadMapping"));
		GamepadMapping->SetDisplayName(LOCTEXT("GamepadMapping_Name", "Gamepad"));

		ControlCollection->AddChildListData(GamepadMapping);
		
		//---------------------------------------------
		// Setting: Gamepad Inputs
		//---------------------------------------------
		{
			FPlayerMappableKeyQueryOptions GamepadOnly;
			GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
			GamepadOnly.bMatchBasicKeyTypes = true;
			
			for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : InputUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, GamepadOnly))
						{
							UGameplayUserListData_KeyRemap* KeyRemap = NewObject<UGameplayUserListData_KeyRemap>();
							KeyRemap->SetDevName(KeyMapping.GetMappingName());
							KeyRemap->SetDisplayName(FText::Format(LOCTEXT("KeyRemap_Name", "{0}"), KeyMapping.GetDisplayName()));
							KeyRemap->InitKeyRemapData(InputUserSettings, MappableKeyProfile, ECommonInputType::Gamepad, KeyMapping);

							GamepadMapping->AddChildListData(KeyRemap);
						}
					}
				}
			}
		}
	}

	RegisteredCollectionTabs.Add(ControlCollection);
}

#undef LOCTEXT_NAMESPACE
