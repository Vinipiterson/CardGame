// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Data/CgTypes.h"
#include "CgCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCgCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CARDGAME_API ICgCombatInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsFromTeam(FGameplayTag InTeamTag) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetTeamTag() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FCgCardDefinition GetCardDefinition() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatTarget(AActor* NewTarget);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetCombatTarget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, DisplayName = "SetFacingWarpTargetLocation")
	void K2_SetFacingWarpTargetLocation(FVector InLocation);
};
