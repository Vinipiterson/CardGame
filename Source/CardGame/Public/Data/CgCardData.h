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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	FText Name = FText::FromString("No Name");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	int32 Cost = 4;
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
	UPROPERTY(EditDefaultsOnly, Category="AbilitySystem")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;
	UPROPERTY(EditDefaultsOnly, Category="AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> InitialGameplayAbilities;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Malee")
	float BaseDamage = 100.f;

	UPROPERTY(EditDefaultsOnly, Category="Malee")
	TArray<TObjectPtr<UAnimMontage>> MaleeMontages;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Malee")
	FName MaleeSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Malee")
	float MaleeTraceRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Malee")
	bool bMaleeSingleHit;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetRandomMaleeMontage() const;
};
