// Copyright Vinipi. All Rights Reserved.

#include "AbilitySystem/Attributes/CgCombatAttributeSet.h"

#include "Net/UnrealNetwork.h"

UCgCombatAttributeSet::UCgCombatAttributeSet()
{
}

void UCgCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Damage, COND_None, REPNOTIFY_Always);
}

void UCgCombatAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCgCombatAttributeSet, Damage, OldDamage);
}