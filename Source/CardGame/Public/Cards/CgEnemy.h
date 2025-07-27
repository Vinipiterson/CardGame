// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Data/CgTypes.h"
#include "GameFramework/Character.h"
#include "CgEnemy.generated.h"

class UCgAbilitySystemComponent;
struct FCgCardDefinition;
class UCgVitalAttributeSet;

UCLASS()
class CARDGAME_API ACgEnemy : public ACharacter, public IAbilitySystemInterface
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
	//~GAS

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~IAbilitySystemInterface

	UPROPERTY(BlueprintReadOnly, Replicated)
	FCgCardDefinition CardDefinition;

	UFUNCTION(BlueprintCallable)
	void SetCardDefinition(FCgCardDefinition InDefinition);
};
