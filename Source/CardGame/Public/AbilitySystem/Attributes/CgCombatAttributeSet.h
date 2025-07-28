// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Data/CgTypes.h"
#include "CgCombatAttributeSet.generated.h"

UCLASS()
class CARDGAME_API UCgCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCgCombatAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing = "OnRep_Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(ThisClass, Damage);

protected:
	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldDamage) const;
};
