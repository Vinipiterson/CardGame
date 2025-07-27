// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CgAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CARDGAME_API UCgAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UCgAbilitySystemComponent();

	void GiveInitialAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, int32 Level);
	void GiveInitialEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects, int32 Level);
};
