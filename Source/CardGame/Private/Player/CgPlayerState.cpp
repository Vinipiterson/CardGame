// Copyright Vinipi. All Rights Reserved.


#include "Player/CgPlayerState.h"

#include "AbilitySystem/Attributes/CgPlayerAttributeSet.h"
#include "Data/CgCardData.h"
#include "Data/CgTags.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/CgAbilitySystemComponent.h"
#include "AbilitySystem/Data/CgCardActivationData.h"

ACgPlayerState::ACgPlayerState()
{
	bReplicates = true;
	SetNetUpdateFrequency(100.f);
	
	ASC = CreateDefaultSubobject<UCgAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCgPlayerAttributeSet>(TEXT("AttributeSet"));
}

void ACgPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACgPlayerState::BeginPlay()
{
	Super::BeginPlay();

	InitializeCards();
}

void ACgPlayerState::InitPlayerState(APawn* AvatarActor)
{
	ASC->InitAbilityActorInfo(this, AvatarActor);

	if (GetLocalRole() == ROLE_Authority)
	{
		ASC->GiveInitialAbilities(InitialGameplayAbilities, Level);
		ASC->GiveInitialEffects(InitialGameplayEffects, Level);
	}
}

UCgPlayerAttributeSet* ACgPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

UAbilitySystemComponent* ACgPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACgPlayerState::InitializeCards()
{
	while (CardsInHand.Num() != 4) // Select Deck
	{
		FCgCardDefinition Card = Deck[FMath::RandRange(0, Deck.Num()-1)];
		if (!CardsInHand.Contains(Card))
		{
			CardsInHand.Add(Card);
		}
	}
	ChooseNextCard();
}

void ACgPlayerState::SetCurrentCard(FCgCardDefinition InCard)
{
	CurrentCard = InCard;

	if (GetLocalRole() != ROLE_Authority)
		ServerSetCurrentCard(InCard);
}

void ACgPlayerState::UseCard(const FTransform& Transform)
{
	SpawnTransform = Transform;

	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(CgAbilitieTags::UseCard));
}

void ACgPlayerState::ServerSetCurrentCard_Implementation(FCgCardDefinition InCard)
{
	SetCurrentCard(InCard);
}

void ACgPlayerState::ChooseNextCard()
{
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

void ACgPlayerState::DiscardCard()
{
	int32 CardID = CardsInHand.Find(CurrentCard);
	CardsInHand[CardID] = NextCard;
	ChooseNextCard();
}
