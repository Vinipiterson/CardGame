// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CgCombatInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "CgTower.generated.h"

class ACgTowerGuardian;
class UCgTowerData;
class UGameplayEffect;
class UGameplayAbility;
class UCgVitalAttributeSet;
class UCgAbilitySystemComponent;

UCLASS()
class CARDGAME_API ACgTower : public AActor, public IAbilitySystemInterface, public ICgCombatInterface
{
	GENERATED_BODY()

public:
	ACgTower();

	virtual void BeginPlay() override;
	
protected:
	//~GAS
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCgAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UCgVitalAttributeSet> VitalAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCgTowerData> TowerData;
	//~GAS

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag TeamTag;

public:
	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~IAbilitySystemInterface

	//~ICgCombatInterface
	virtual bool IsFromTeam_Implementation(FGameplayTag InTeamTag) const override;
	virtual FGameplayTag GetTeamTag_Implementation() const override;
	//~ICgCombatInterface
};
