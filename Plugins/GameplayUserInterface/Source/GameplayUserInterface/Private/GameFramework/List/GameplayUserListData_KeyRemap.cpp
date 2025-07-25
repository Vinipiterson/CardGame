// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/List/GameplayUserListData_KeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserListData_KeyRemap, Log, All);

UGameplayUserListData_KeyRemap::UGameplayUserListData_KeyRemap()
{
	InputUserSettings = nullptr;
	PlayerMappableKeyProfile = nullptr;

	DesiredInputKeyType = ECommonInputType::MouseAndKeyboard;
	MappableKeySlot = EPlayerMappableKeySlot::Unspecified;

	MappingName = NAME_None;
}

void UGameplayUserListData_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* InInputUserSettings, UEnhancedPlayerMappableKeyProfile* InPlayerMappableKeyProfile,
	ECommonInputType InDesiredInputKeyType, const FPlayerKeyMapping& InPlayerKeyMapping)
{
	InputUserSettings = InInputUserSettings;
	PlayerMappableKeyProfile = InPlayerMappableKeyProfile;

	DesiredInputKeyType = InDesiredInputKeyType;
	MappableKeySlot = InPlayerKeyMapping.GetSlot();

	MappingName = InPlayerKeyMapping.GetMappingName();
}

FSlateBrush UGameplayUserListData_KeyRemap::GetIconFromCurrentKey() const
{
	check(InputUserSettings.Get());
	
	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(InputUserSettings.Get()->GetLocalPlayer());
	check(CommonInputSubsystem);

	FSlateBrush OutBrush;
	if (!UCommonInputPlatformSettings::Get()->TryGetInputBrush(OutBrush, GetOwningKeyMapping()->GetCurrentKey(), DesiredInputKeyType, CommonInputSubsystem->GetCurrentGamepadName()))
	{
		UE_LOG(LogGameplayUserListData_KeyRemap, Error, TEXT("Unable fin an icon for the key [%s]. Empty brush applied."), *GetOwningKeyMapping()->GetCurrentKey().GetDisplayName().ToString());
	}

	return OutBrush;
}

void UGameplayUserListData_KeyRemap::BindNewKey(const FKey& NewKey)
{
	check(InputUserSettings.Get());

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = MappingName;
	KeyArgs.Slot = MappableKeySlot;
	KeyArgs.NewKey = NewKey;

	FGameplayTagContainer FailureTags;
	InputUserSettings->MapPlayerKey(KeyArgs, FailureTags);
	InputUserSettings->SaveSettings();

	NotifyListDataModified(this);
}

bool UGameplayUserListData_KeyRemap::HasDefaultValue() const
{
	return GetOwningKeyMapping()->GetDefaultKey().IsValid();
}

bool UGameplayUserListData_KeyRemap::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && GetOwningKeyMapping()->IsCustomized();
}

bool UGameplayUserListData_KeyRemap::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		check(InputUserSettings.Get());
		
		GetOwningKeyMapping()->ResetToDefault();
		InputUserSettings.Get()->SaveSettings();
		NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

		return true;
	}

	return false;
}

FPlayerKeyMapping* UGameplayUserListData_KeyRemap::GetOwningKeyMapping() const
{
	check(PlayerMappableKeyProfile.Get());

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = MappingName;
	KeyArgs.Slot = MappableKeySlot;
	
	return PlayerMappableKeyProfile.Get()->FindKeyMapping(KeyArgs);
}
