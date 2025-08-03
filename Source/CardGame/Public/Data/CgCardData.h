// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CgCardData.generated.h"

class ACgSpell;
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
	FGameplayTag CardType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	int32 NumToSpawn = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	TSubclassOf<ACharacter> AIClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	FGameplayTag TerrainTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	FGameplayTagContainer AttackingTerrainTags;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	TArray<TSubclassOf<UGameplayAbility>> InitialGameplayAbilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spell")
	TSubclassOf<ACgSpell> SpellClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spell")
	TSubclassOf<UGameplayEffect> PlayerDamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category="Enemy|Attack")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack")
	FName AttackSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Malee")
	float MaleeTraceRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Malee")
	bool bMaleeSingleHit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Ranged")
	TSubclassOf<ACgProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetRandomAttackMontage() const;
};
