// Copyright Vinipi. All Rights Reserved.

#include "AI/CgAIController.h"

#include "Components/StateTreeAIComponent.h"

ACgAIController::ACgAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("State Tree"));
}

