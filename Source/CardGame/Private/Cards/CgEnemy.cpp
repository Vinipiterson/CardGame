// Copyright Vinipi. All Rights Reserved.

#include "Cards/CgEnemy.h"

#include "AbilitySystem/CgAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CgVitalAttributeSet.h"
#include "AbilitySystem/Attributes/CgPlayerAttributeSet.h"
#include "Data/CgCardData.h"
#include "Net/UnrealNetwork.h"

ACgEnemy::ACgEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ASC = CreateDefaultSubobject<UCgAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	VitalAttributeSet = CreateDefaultSubobject<UCgVitalAttributeSet>(TEXT("AttributeSet"));
}

void ACgEnemy::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CardDefinition);
}

UAbilitySystemComponent* ACgEnemy::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACgEnemy::SetCardDefinition(FCgCardDefinition InDefinition)
{
	CardDefinition = InDefinition;

	ASC->InitAbilityActorInfo(this, this);

	if (GetLocalRole() == ROLE_Authority)
	{
		ASC->GiveInitialAbilities(CardDefinition.CardData->InitialGameplayAbilities, CardDefinition.Level);
		ASC->GiveInitialEffects(CardDefinition.CardData->InitialGameplayEffects, CardDefinition.Level);
	}
}
