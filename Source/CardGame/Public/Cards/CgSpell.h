// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Data/CgTypes.h"
#include "GameFramework/Actor.h"
#include "CgSpell.generated.h"

UCLASS()
class CARDGAME_API ACgSpell : public AActor
{
	GENERATED_BODY()

public:
	//~Combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FCgCardDefinition CardDefinition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FGameplayTag TeamTag;

	UPROPERTY(BlueprintReadOnly, Category="Projectile|Damage", Meta = (ExposeOnSpawn=true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
	//~Combat
};
