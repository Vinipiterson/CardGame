// Copyright Vinipi. All Rights Reserved.

#include "AbilitySystem/Attributes/CgVitalAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Data/CgTags.h"
#include "Interfaces/CgVitalInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

UCgVitalAttributeSet::UCgVitalAttributeSet()
{
}

void UCgVitalAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UCgVitalAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UCgVitalAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (Data.EvaluatedData.Magnitude < 0.f) // Lost Health
		{
			if (GetHealth() <= 0.f)
			{
				GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(CgAbilityTags::Die));
			}
			else
			{
				GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(CgAbilityTags::HitReact));
			}
		}
	}
}

void UCgVitalAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UCgVitalAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCgVitalAttributeSet, Health, OldHealth);
}

void UCgVitalAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCgVitalAttributeSet, MaxHealth, OldMaxHealth);
}