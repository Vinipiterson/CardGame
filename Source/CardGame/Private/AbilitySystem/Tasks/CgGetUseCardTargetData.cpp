// Copyright Vinipi. All Rights Reserved.


#include "AbilitySystem/Tasks/CgGetUseCardTargetData.h"

#include "AbilitySystemComponent.h"
#include "Player/CgPlayerController.h"

UCgGetUseCardTargetData* UCgGetUseCardTargetData::CreateGetUseCardTargetData(UGameplayAbility* OwningAbility)
{
	UCgGetUseCardTargetData* Obj = NewAbilityTask<UCgGetUseCardTargetData>(OwningAbility);
	return Obj;
}

void UCgGetUseCardTargetData::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UCgGetUseCardTargetData::SendMouseCursorData()
{
	if (ACgPlayerController* Controller = Cast<ACgPlayerController>(Ability->GetCurrentActorInfo()->PlayerController))
	{
		FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
			
		FGameplayAbilityTargetDataHandle DataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

		FHitResult CursorHit;
		Controller->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
		Data->HitResult = CursorHit;
		
		DataHandle.Add(Data);
			
		AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
			GetActivationPredictionKey(),
			DataHandle, FGameplayTag(),
			AbilitySystemComponent->ScopedPredictionKey);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			ValidData.Broadcast(DataHandle);
		}
	}
}

void UCgGetUseCardTargetData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
