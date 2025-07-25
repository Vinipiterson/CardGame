// Copyright Vinipi. All Rights Reserved.

#include "Player/CgPlayerController.h"

ACgPlayerController::ACgPlayerController()
{
	bAutoManageActiveCameraTarget = false;

	bShowMouseCursor = true;
}

void ACgPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetViewTarget(GetViewTargetForPlayer(GetLocalRole() == ROLE_Authority ? 1 : 2));
}
