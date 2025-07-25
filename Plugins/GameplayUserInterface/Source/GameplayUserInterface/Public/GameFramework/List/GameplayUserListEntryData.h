// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayUserListEntryData.generated.h"

class UGameplayUserListEntryWidget;
class UGameplayUserListData;

UCLASS(BlueprintType, Blueprintable, meta=(DisplayName = "Gameplay User Interface: List Entry"))
class GAMEPLAYUSERINTERFACE_API UGameplayUserListEntryData : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UGameplayUserListEntryWidget> FindEntryWidgetClassByListData(UGameplayUserListData* ListData) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Entry")
	TMap<TSubclassOf<UGameplayUserListData>, TSubclassOf<UGameplayUserListEntryWidget>> ListEntryDataMap;
};
