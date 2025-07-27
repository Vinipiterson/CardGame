// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CgTypes.h"
#include "UObject/Object.h"
#include "CgCardActivationData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CARDGAME_API UCgCardActivationData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FCgCardDefinition CardDefinition;

	UPROPERTY(BlueprintReadOnly)
	FTransform Transform;
};
