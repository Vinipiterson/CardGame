// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CgTypes.generated.h"

class UCgCardData;

USTRUCT(Blueprintable)
struct FCgCardDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCgCardData> CardData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level;

	bool operator==(const FCgCardDefinition& I) const;
};