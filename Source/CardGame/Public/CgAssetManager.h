// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CgAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class CARDGAME_API UCgAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UCgAssetManager& Get();

	virtual void StartInitialLoading() override;
};
