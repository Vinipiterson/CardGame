// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CgTypes.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CgCombatInterface.h"
#include "CgPlayerState.generated.h"

class UCgAbilitySystemComponent;
class UGameplayAbility;
class UCgPlayerAttributeSet;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardsUpdatedSignature);

UCLASS()
class CARDGAME_API ACgPlayerState : public APlayerState, public IAbilitySystemInterface, public ICgCombatInterface
{
	GENERATED_BODY()

public:
	ACgPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	void InitPlayerState(APawn* AvatarActor);

protected:
	//~Level
	int32 Level;
	//~Level
	
	//~GAS
	UPROPERTY()
	TObjectPtr<UCgAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UCgPlayerAttributeSet> AttributeSet;
	UPROPERTY(EditDefaultsOnly, Category="Player|AbilitySystem")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;
	UPROPERTY(EditDefaultsOnly, Category="Player|AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> InitialGameplayAbilities;

public:
	UFUNCTION(BlueprintCallable)
	UCgPlayerAttributeSet* GetAttributeSet() const;
	//~GAS

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~IAbilitySystemInterface
	
	//~ICgCombatInterface
	virtual bool IsFromTeam_Implementation(FGameplayTag InTeamTag) const override;
	virtual FGameplayTag GetTeamTag_Implementation() const override;
	//~ICgCombatInterface
	
	//~Cards
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Elixir = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCgCardDefinition> Deck;
	UPROPERTY(BlueprintReadOnly)
	TArray<FCgCardDefinition> CardsInHand;
	UPROPERTY(BlueprintReadOnly)
	FCgCardDefinition NextCard;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FCgCardDefinition CurrentCard;
	UPROPERTY(Replicated, BlueprintReadOnly)
	FTransform SpawnTransform;

public:
	UPROPERTY(BlueprintAssignable)
	FOnCardsUpdatedSignature OnCardsUpdated;

	UFUNCTION(BlueprintCallable)
	FCgCardDefinition GetCurrentCard() const { return CurrentCard; }
	UFUNCTION(BlueprintCallable)
	FTransform GetSpawnTransform() const { return SpawnTransform; }

protected:
	void InitializeCards();
	void ChooseNextCard();
	UFUNCTION(BlueprintCallable)
	void DiscardCard();

	UFUNCTION(BlueprintCallable)
	void SetCurrentCard(FCgCardDefinition InCard);
	UFUNCTION(Server, Reliable)
	void ServerSetCurrentCard(FCgCardDefinition InCard);

public:
	UFUNCTION(BlueprintCallable)
	void UseCard(const FTransform& Transform);
	//~Cards
};
