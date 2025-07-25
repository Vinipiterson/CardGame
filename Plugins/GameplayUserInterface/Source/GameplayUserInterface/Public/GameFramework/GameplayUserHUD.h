// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "GameplayUserHUD.generated.h"

UCLASS()
class GAMEPLAYUSERINTERFACE_API AGameplayUserHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameplayUserHUD(const FObjectInitializer& ObjectInitializer);
};
