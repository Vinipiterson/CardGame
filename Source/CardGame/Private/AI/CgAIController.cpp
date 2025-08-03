// Copyright Vinipi. All Rights Reserved.

#include "AI/CgAIController.h"

#include "CgCombatInterface.h"
#include "Components/StateTreeAIComponent.h"
#include "Data/CgTags.h"
#include "Navigation/CrowdFollowingComponent.h"

ACgAIController::ACgAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("State Tree"));
}

void ACgAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UCrowdFollowingComponent* Component = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Component->SetCrowdSimulationState(ECrowdSimulationState::Enabled);

		if (AvoidanceQuality == 1)
			Component->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
		else if (AvoidanceQuality == 2)
			Component->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
		else if (AvoidanceQuality == 3)
			Component->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
		else if (AvoidanceQuality == 4)
			Component->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
		
		Component->SetCrowdCollisionQueryRange(AvoidanceQueryRange);
		Component->SetCrowdSeparation(true);
		Component->SetCrowdSeparationWeight(AvoidanceSeparationWeight);

		FGameplayTag TeamTag = ICgCombatInterface::Execute_GetTeamTag(InPawn);
		if (TeamTag.MatchesTagExact(CgTeamTags::Team1))
		{
			Component->SetAvoidanceGroup(1);
			Component->SetGroupsToAvoid(2);
		}
		else
		{
			Component->SetAvoidanceGroup(2);
			Component->SetGroupsToAvoid(1);
		}
	}
}

