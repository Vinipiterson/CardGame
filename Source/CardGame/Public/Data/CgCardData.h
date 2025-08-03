// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CgCardData.generated.h"

class ACgProjectile;
class UGameplayAbility;
class UGameplayEffect;

UCLASS(AutoCollapseCategories=("Attack|Ranged", "Attack|Malee"))
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
	FGameplayTag EnemyType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	FGameplayTagContainer AttackingTerrainTags;
	UPROPERTY(EditDefaultsOnly, Category="AbilitySystem")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;
	UPROPERTY(EditDefaultsOnly, Category="AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> InitialGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	FName AttackSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Malee")
	float MaleeTraceRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Malee")
	bool bMaleeSingleHit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Ranged")
	TSubclassOf<ACgProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetRandomAttackMontage() const;
};
