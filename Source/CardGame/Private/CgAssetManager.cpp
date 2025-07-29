// Copyright Vinipi. All Rights Reserved.


#include "CgAssetManager.h"

#include "AbilitySystemGlobals.h"

UCgAssetManager& UCgAssetManager::Get()
{
	check(GEngine);

	UCgAssetManager* AM = Cast<UCgAssetManager>(GEngine->AssetManager);
	return *AM;
}

void UCgAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}
