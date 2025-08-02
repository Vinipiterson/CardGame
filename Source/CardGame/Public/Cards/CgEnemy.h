// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CgCombatInterface.h"
#include "GameplayTagContainer.h"
#include "Data/CgTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/CgVitalInterface.h"
#include "CgEnemy.generated.h"

class UCgGameplayAbility;
class UCgCombatAttributeSet;
class UCgAbilitySystemComponent;
struct FCgCardDefinition;
class UCgVitalAttributeSet;

UCLASS()
class CARDGAME_API ACgEnemy : public ACharacter, public IAbilitySystemInterface, public ICgCombatInterface, public ICgVitalInterface
{
	GENERATED_BODY()

public:
	ACgEnemy();

	//~AActor
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	//~AActor

protected:
	//~GAS
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCgAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UCgVitalAttributeSet> VitalAttributeSet;
	UPROPERTY()
	TObjectPtr<UCgCombatAttributeSet> CombatAttributeSet;
	//~GAS

public:
	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~IAbilitySystemInterface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta=(ExposeOnSpawn))
	FCgCardDefinition CardDefinition;

	//~Team
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta=(ExposeOnSpawn))
	FGameplayTag TeamTag;
	//~Team

	//~ICgCombatInterface
	virtual FGameplayTag GetTeamTag_Implementation() const override;
	virtual bool IsFromTeam_Implementation(FGameplayTag InTeamTag) const override;
	virtual FCgCardDefinition GetCardDefinition_Implementation() const override;
	//~ICgCombatInterface

	//~ICgCombatInterface
	virtual void NotifyDeath_Implementation() override;
	virtual USoundBase* GetDeathSound_Implementation() override;
	virtual USoundBase* GetHitSound_Implementation() override;
	virtual AActor* GetCombatTarget_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* NewTarget) override;
	//~ICgCombatInterface

	//~Combat
	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
	//~Combat

	//~Death
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> DeathSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> HitSound;
	//~Death
};
