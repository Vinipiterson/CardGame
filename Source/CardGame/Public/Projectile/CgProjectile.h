// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "CgProjectile.generated.h"

UCLASS()
class CARDGAME_API ACgProjectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Projectile|Damage", Meta = (ExposeOnSpawn=true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* OtherActor);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	FGameplayTag ImpactCueTag;
	UPROPERTY(BlueprintReadOnly)
	bool bAlreadyHit = false;

	void ImpactFX(AActor* Target) const;
};
