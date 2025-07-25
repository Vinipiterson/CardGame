// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayUserListData.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "CommonInputTypeEnum.h"
#include "GameplayUserListData_KeyRemap.generated.h"

class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_KeyRemap : public UGameplayUserListData
{
	GENERATED_BODY()
	
public:
	UGameplayUserListData_KeyRemap();

	virtual void InitKeyRemapData(UEnhancedInputUserSettings* InInputUserSettings, UEnhancedPlayerMappableKeyProfile* InPlayerMappableKeyProfile, ECommonInputType InDesiredInputKeyType, const FPlayerKeyMapping& InPlayerKeyMapping);

	FSlateBrush GetIconFromCurrentKey() const;
	FORCEINLINE ECommonInputType GetDesiredInputKeyType() const { return DesiredInputKeyType; }

	void BindNewKey(const FKey& NewKey);

	//~Begin UGameplayUserListData interface
	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	//~End of UGameplayUserListData interface
	
private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UEnhancedInputUserSettings> InputUserSettings;

	UPROPERTY(Transient)
	TWeakObjectPtr<UEnhancedPlayerMappableKeyProfile> PlayerMappableKeyProfile;

	ECommonInputType DesiredInputKeyType;
	EPlayerMappableKeySlot MappableKeySlot;
	
	FName MappingName;

private:
	FPlayerKeyMapping* GetOwningKeyMapping() const;
};
