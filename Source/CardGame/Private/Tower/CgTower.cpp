// Copyright Vinipi. All Rights Reserved.

#include "Tower/CgTower.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CgAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CgVitalAttributeSet.h"
#include "Data/CgTowerData.h"

ACgTower::ACgTower()
{
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UCgAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	VitalAttributeSet = CreateDefaultSubobject<UCgVitalAttributeSet>(TEXT("AttributeSet"));
}

void ACgTower::BeginPlay()
{
	Super::BeginPlay();

	ASC->InitAbilityActorInfo(this, this);
	ASC->GiveInitialAbilities(TowerData->InitialGameplayAbilities, 1);
	ASC->GiveInitialEffects(TowerData->InitialGameplayEffects, 1);
}

bool ACgTower::IsFromTeam_Implementation(FGameplayTag InTeamTag) const
{
	return TeamTag.MatchesTagExact(InTeamTag);
}

FGameplayTag ACgTower::GetTeamTag_Implementation() const
{
	return TeamTag;
}

UAbilitySystemComponent* ACgTower::GetAbilitySystemComponent() const
{
	return ASC;
}
