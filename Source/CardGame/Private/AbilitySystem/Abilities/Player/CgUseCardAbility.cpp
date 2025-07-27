// Copyright Vinipi. All Rights Reserved.


#include "AbilitySystem/Abilities/Player/CgUseCardAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Attributes/CgPlayerAttributeSet.h"
#include "Data/CgCardData.h"
#include "Data/CgTags.h"
#include "Player/CgPlayerState.h"

bool UCgUseCardAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  FGameplayTagContainer* OptionalRelevantTags) const
{
	ACgPlayerState* PS = Cast<ACgPlayerState>(ActorInfo->OwnerActor);
	float Cost = PS->GetCurrentCard().CardData->Cost;
	
	UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (CostGE)
	{
		UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
		if (ensure(ASC))
		{
			const UCgPlayerAttributeSet* AS = Cast<UCgPlayerAttributeSet>(ASC->GetAttributeSet(UCgPlayerAttributeSet::StaticClass()));
			if (AS->GetElixir() < Cost)
				return false;
		}
	}
	return true;
}

void UCgUseCardAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	ACgPlayerState* PS = Cast<ACgPlayerState>(ActorInfo->OwnerActor);
	float Cost = PS->GetCurrentCard().CardData->Cost;

	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (ensure(ASC))
	{
		auto Context = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(CostGameplayEffectClass, GetAbilityLevel(), Context);
		Spec.Data->SetByCallerTagMagnitudes.Add(CgSetByCallerTags::Cost, -Cost);
		
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
	}
}
