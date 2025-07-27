// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Data/CgTypes.h"
#include "CgPlayerAttributeSet.generated.h"

UCLASS()
class CARDGAME_API UCgPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCgPlayerAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category="Player|Attributes", ReplicatedUsing = "OnRep_Elixir")
	FGameplayAttributeData Elixir;
	ATTRIBUTE_ACCESSORS(ThisClass, Elixir);
	
	UPROPERTY(BlueprintReadOnly, Category="Player|Attributes", ReplicatedUsing = "OnRep_MaxElixir")
	FGameplayAttributeData MaxElixir;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxElixir);

protected:
	UFUNCTION()
	void OnRep_Elixir(const FGameplayAttributeData& OldElixir);
	UFUNCTION()
	void OnRep_MaxElixir(const FGameplayAttributeData& OldMaxElixir);
};
