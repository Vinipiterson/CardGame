// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "CgTower.generated.h"

class UCgTowerData;
class UGameplayEffect;
class UGameplayAbility;
class UCgVitalAttributeSet;
class UCgAbilitySystemComponent;

UCLASS()
class CARDGAME_API ACgTower : public AActor, public IAbilitySystemInterface
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
	int32 PlayerID = 1;

public:
	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~IAbilitySystemInterface
};
