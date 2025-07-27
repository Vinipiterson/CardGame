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

void ACgPlayerPawn::UseCard_Implementation(const FTransform& Transform)
{
	ACgPlayerState* PS = GetPlayerState<ACgPlayerState>();
	check(PS)

	PS->UseCard(Transform);
}
