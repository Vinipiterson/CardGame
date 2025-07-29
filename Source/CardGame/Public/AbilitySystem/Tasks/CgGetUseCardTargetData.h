// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CgGetUseCardTargetData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUseCardTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class CARDGAME_API UCgGetUseCardTargetData : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="GetUseCardTargetData", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly=true))
	static UCgGetUseCardTargetData* CreateGetUseCardTargetData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FUseCardTargetDataSignature ValidData;

private:

	virtual void Activate() override;

	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
