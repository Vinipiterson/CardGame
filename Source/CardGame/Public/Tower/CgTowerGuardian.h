// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CgCombatInterface.h"
#include "GameFramework/Pawn.h"
#include "CgTowerGuardian.generated.h"

class UCgCombatAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UCgAbilitySystemComponent;

UCLASS()
class CARDGAME_API ACgTowerGuardian : public APawn, public ICgCombatInterface
{
	GENERATED_BODY()

public:
	ACgTowerGuardian();

protected:
	virtual void BeginPlay() override;

	//~GAS
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCgAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UCgCombatAttributeSet> CombatAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> InitialGameplayAbilities;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;
	//~GAS

	//~ICgCombatInterface
	virtual AActor* GetCombatTarget_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* NewTarget) override;
	virtual FGameplayTag GetTeamTag_Implementation() const override;
	virtual bool IsFromTeam_Implementation(FGameplayTag InTeamTag) const override;
	//~ICgCombatInterface

	//~Combat
	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn))
	FGameplayTag TeamTag;
	//~Combat
};
