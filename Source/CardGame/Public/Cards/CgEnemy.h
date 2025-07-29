// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CgCombatInterface.h"
#include "GameplayTagContainer.h"
#include "Data/CgTypes.h"
#include "GameFramework/Character.h"
#include "CgEnemy.generated.h"

class UCgCombatAttributeSet;
class UCgAbilitySystemComponent;
struct FCgCardDefinition;
class UCgVitalAttributeSet;

UCLASS()
class CARDGAME_API ACgEnemy : public ACharacter, public IAbilitySystemInterface, public ICgCombatInterface
{
	GENERATED_BODY()

public:
	ACgEnemy();

	//~AActor
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//~AActor
	
	//~GAS
	UPROPERTY()
	TObjectPtr<UCgAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UCgVitalAttributeSet> VitalAttributeSet;
	UPROPERTY()
	TObjectPtr<UCgCombatAttributeSet> CombatAttributeSet;
	//~GAS

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~IAbilitySystemInterface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta=(ExposeOnSpawn))
	FCgCardDefinition CardDefinition;

	UFUNCTION(BlueprintCallable)
	void SetCardDefinition(FCgCardDefinition InDefinition);

	//~Team
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta=(ExposeOnSpawn))
	FGameplayTag TeamTag;
	//~Team

	//~ICgCombatInterface
	virtual FGameplayTag GetTeamTag_Implementation() const override;
	virtual bool IsFromTeam_Implementation(FGameplayTag InTeamTag) const override;
	//~ICgCombatInterface
};
