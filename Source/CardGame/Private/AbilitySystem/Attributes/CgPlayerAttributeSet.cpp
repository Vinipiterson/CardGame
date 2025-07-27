// Copyright Vinipi. All Rights Reserved.


#include "AbilitySystem/Attributes/CgPlayerAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UCgPlayerAttributeSet::UCgPlayerAttributeSet()
{
}

void UCgPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Elixir, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxElixir, COND_None, REPNOTIFY_Always);
}

void UCgPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetElixirAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxElixir());
	}
}

void UCgPlayerAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UCgPlayerAttributeSet::OnRep_Elixir(const FGameplayAttributeData& OldElixir)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCgPlayerAttributeSet, Elixir, OldElixir);
}

void UCgPlayerAttributeSet::OnRep_MaxElixir(const FGameplayAttributeData& OldMaxElixir)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCgPlayerAttributeSet, MaxElixir, OldMaxElixir);
}
