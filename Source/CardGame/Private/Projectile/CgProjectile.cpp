// Copyright Vinipi. All Rights Reserved.

#include "Projectile/CgProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CgCombatInterface.h"
#include "GameplayCueFunctionLibrary.h"
#include "GameplayEffect.h"

void ACgProjectile::OnOverlap(AActor* OtherActor)
{
	if (GetLocalRole() != ROLE_Authority)
		return;
	
	if (bAlreadyHit)
		return;
	
	if (!DamageEffectSpecHandle.Data.IsValid())
		return;
	
	AActor* Causer = DamageEffectSpecHandle.Data->GetContext().GetEffectCauser();

	if (!IsValid(Causer))
		return;
	
	if (OtherActor == Causer)
		return;

	const FGameplayTag CauserTeam = ICgCombatInterface::Execute_GetTeamTag(Causer);
	if (!ICgCombatInterface::Execute_IsFromTeam(OtherActor, CauserTeam))
	{
		bAlreadyHit = true;
		
		ImpactFX(OtherActor);
		
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
		}
		
		Destroy();
	}
}

void ACgProjectile::ImpactFX(AActor* Target) const
{
	FGameplayCueParameters Params{};
	UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(Target, ImpactCueTag, Params);
}
