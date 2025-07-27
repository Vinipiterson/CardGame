// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CgPlayerPawn.generated.h"

UCLASS()
class CARDGAME_API ACgPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	ACgPlayerPawn();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UseCard(const FTransform& Transform);
};
