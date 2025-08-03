// Copyright Vinipi. All Rights Reserved.

#include "Tower/CgTowerGuardian.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CgAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CgCombatAttributeSet.h"

ACgTowerGuardian::ACgTowerGuardian()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	ASC = CreateDefaultSubobject<UCgAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	CombatAttributeSet = CreateDefaultSubobject<UCgCombatAttributeSet>(TEXT("CombatAttributeSet"));
}

void ACgTowerGuardian::BeginPlay()
{
	Super::BeginPlay();

	ASC->InitAbilityActorInfo(this, this);

	if (GetLocalRole() == ROLE_Authority)
	{
		ASC->GiveInitialAbilities(InitialGameplayAbilities, 1);
		ASC->GiveInitialEffects(InitialGameplayEffects, 1);
	}
}

AActor* ACgTowerGuardian::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

void ACgTowerGuardian::SetCombatTarget_Implementation(AActor* NewTarget)
{
	CombatTarget = NewTarget;
}

FGameplayTag ACgTowerGuardian::GetTeamTag_Implementation() const
{
	return TeamTag;
}

bool ACgTowerGuardian::IsFromTeam_Implementation(FGameplayTag InTeamTag) const
{
	return TeamTag.MatchesTagExact(InTeamTag);
}
