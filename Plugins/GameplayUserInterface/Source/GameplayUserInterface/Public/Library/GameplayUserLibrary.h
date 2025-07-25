// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayUserLibrary.generated.h"

class UGameplayUserActivatableWidget;
class UGameplayUserConfirmationDialogWidget;

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Gameplay User Library")
	static TSoftClassPtr<UGameplayUserActivatableWidget> GetGameplayUserScreenByTag(UPARAM(meta = (Categories = "GameplayUser.Screen"))FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Gameplay User Library")
	static TSoftObjectPtr<UTexture2D> GetGameplayUserDescriptionImageByTag(UPARAM(meta = (Categories = "GameplayUser.Description"))FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Gameplay User Library")
	static TSoftClassPtr<UGameplayUserConfirmationDialogWidget> GetConfirmationDialogWidget();
};
