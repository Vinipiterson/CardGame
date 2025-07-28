// Copyright Vinipi. All Rights Reserved.

#include "Cards/CgEnemy.h"

#include "AbilitySystem/CgAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CgCombatAttributeSet.h"
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

	VitalAttributeSet = CreateDefaultSubobject<UCgVitalAttributeSet>(TEXT("VitalAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<UCgCombatAttributeSet>(TEXT("CombatAttributeSet"));
}

void ACgEnemy::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CardDefinition);
	DOREPLIFETIME(ThisClass, TeamTag);
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

FGameplayTag ACgEnemy::GetTeamTag_Implementation() const
{
	return TeamTag;
}

bool ACgEnemy::IsFromTeam_Implementation(FGameplayTag InTeamTag) const
{
	return TeamTag.MatchesTagExact(InTeamTag);
}
