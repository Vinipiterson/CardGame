// Copyright Vinipi. All Rights Reserved.

#include "Player/CgPlayerPawn.h"

#include "Player/CgPlayerState.h"

ACgPlayerPawn::ACgPlayerPawn()
{
	bReplicates = true;
}

void ACgPlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ACgPlayerState* PS = GetPlayerState<ACgPlayerState>();
	check(PS)
	
	PS->InitPlayerState(this);
}

void ACgPlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ACgPlayerState* PS = GetPlayerState<ACgPlayerState>();
	check(PS)

	PS->InitPlayerState(this);
}

bool ACgPlayerPawn::IsFromTeam_Implementation(FGameplayTag InTeamTag) const
{
	return ICgCombatInterface::Execute_IsFromTeam(Controller, InTeamTag);
}

FGameplayTag ACgPlayerPawn::GetTeamTag_Implementation() const
{
	return ICgCombatInterface::Execute_GetTeamTag(Controller);
}

void ACgPlayerPawn::UseCard_Implementation(const FTransform& Transform)
{
	ACgPlayerState* PS = GetPlayerState<ACgPlayerState>();
	check(PS)

	PS->UseCard(Transform);
}
