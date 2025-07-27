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

	DOREPLIFETIME(ThisClass, Team);
}

void ACgPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//SetViewTarget(GetViewTargetForPlayer(GetLocalRole() == ROLE_Authority ? 1 : 2));
}
