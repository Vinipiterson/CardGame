// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CgCardData.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UCLASS()
class CARDGAME_API UCgCardData : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	FText Name = FText::FromString("No Name");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	float Cost = 4.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	TSubclassOf<ACharacter> AIClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	int32 NumToSpawn = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	FGameplayTag TerrainTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	FGameplayTagContainer AttackingTerrainTags;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGive;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply;
};
