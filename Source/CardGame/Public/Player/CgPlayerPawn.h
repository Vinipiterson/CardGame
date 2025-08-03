// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CgCombatInterface.h"
#include "GameFramework/Pawn.h"
#include "CgPlayerPawn.generated.h"

UCLASS()
class CARDGAME_API ACgPlayerPawn : public APawn, public ICgCombatInterface
{
	GENERATED_BODY()

public:
	ACgPlayerPawn();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UseCard();

	//~ICgCombatInterface
	virtual bool IsFromTeam_Implementation(FGameplayTag InTeamTag) const override;
	virtual FGameplayTag GetTeamTag_Implementation() const override;
	//~ICgCombatInterface
};
