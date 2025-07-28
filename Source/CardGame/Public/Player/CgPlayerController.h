// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CgCombatInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "CgPlayerController.generated.h"

UCLASS()
class CARDGAME_API ACgPlayerController : public APlayerController, public ICgCombatInterface
{
	GENERATED_BODY()

public:
	ACgPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~ICgCombatInterface
	virtual bool IsFromTeam_Implementation(FGameplayTag InTeamTag) const override;
	virtual FGameplayTag GetTeamTag_Implementation() const override;
	//~ICgCombatInterface
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	FGameplayTag TeamTag;
};
