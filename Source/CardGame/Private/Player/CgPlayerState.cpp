// Copyright Vinipi. All Rights Reserved.


#include "Player/CgPlayerState.h"

void ACgPlayerState::BeginPlay()
{
	Super::BeginPlay();

	while (CardsInHand.Num() != 4) // Select Deck
	{
		FCgCardDefinition Card = Deck[FMath::RandRange(0, Deck.Num()-1)];
		if (!CardsInHand.Contains(Card))
		{
			CardsInHand.Add(Card);
		}
	}
	while (true) // Select Next Card
	{
		FCgCardDefinition Card = Deck[FMath::RandRange(0, Deck.Num()-1)];
		if (!CardsInHand.Contains(Card))
		{
			NextCard = Card;
			break;
		}
	}
}

void ACgPlayerState::UseCard(FCgCardDefinition CardDefinition, const FVector& Location)
{
	int32 Index = CardsInHand.Find(CardDefinition);
	CardsInHand[Index] = NextCard;
	while (true) // Select Next Card
	{
		FCgCardDefinition Card = Deck[FMath::RandRange(0, Deck.Num()-1)];
		if (!CardsInHand.Contains(Card))
		{
			NextCard = Card;
			break;
		}
	}

	OnCardsUpdated.Broadcast();
}
