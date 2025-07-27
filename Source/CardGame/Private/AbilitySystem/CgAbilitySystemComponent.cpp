// Copyright Vinipi. All Rights Reserved.

#include "AbilitySystem/CgAbilitySystemComponent.h"

UCgAbilitySystemComponent::UCgAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCgAbilitySystemComponent::GiveInitialAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, int32 Level)
{
	for (TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Ability, Level);
		Spec.SourceObject = this;
		GiveAbility(Spec);
	}
}

void UCgAbilitySystemComponent::GiveInitialEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects, int32 Level)
{
	FGameplayEffectContextHandle Context = MakeEffectContext();
	for (auto Effect : Effects)
	{
		FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(Effect, Level, Context);
		ApplyGameplayEffectSpecToSelf(*Spec.Data);	
	}
}
