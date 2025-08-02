// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CgVitalInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCgVitalInterface : public UInterface
{
	GENERATED_BODY()
};

class CARDGAME_API ICgVitalInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void NotifyDeath();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USoundBase* GetDeathSound();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USoundBase* GetHitSound();
};
