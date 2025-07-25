// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CgTypes.h"
#include "GameFramework/PlayerState.h"
#include "CgPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardsUpdatedSignature);

UCLASS()
class CARDGAME_API ACgPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Elixir = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCgCardDefinition> Deck;
	UPROPERTY(BlueprintReadOnly)
	TArray<FCgCardDefinition> CardsInHand;
	UPROPERTY(BlueprintReadOnly)
	FCgCardDefinition NextCard;

public:
	UPROPERTY(BlueprintAssignable)
	FOnCardsUpdatedSignature OnCardsUpdated;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void UseCard(FCgCardDefinition CardDefinition, const FVector& Location);
};
