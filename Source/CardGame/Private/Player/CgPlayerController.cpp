// Copyright Vinipi. All Rights Reserved.

#include "Player/CgPlayerController.h"

#include "Net/UnrealNetwork.h"
#include "Player/CgPlayerState.h"

ACgPlayerController::ACgPlayerController()
{
	bAutoManageActiveCameraTarget = false;

	bShowMouseCursor = true;
}

void ACgPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamTag);
}

bool ACgPlayerController::IsFromTeam_Implementation(FGameplayTag InTeamTag) const
{
	return TeamTag.MatchesTagExact(InTeamTag);
}

FGameplayTag ACgPlayerController::GetTeamTag_Implementation() const
{
	return TeamTag;
}

void ACgPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//SetViewTarget(GetViewTargetForPlayer(GetLocalRole() == ROLE_Authority ? 1 : 2));
}
