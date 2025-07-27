// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "AttributeSet.h"
#include "CgTypes.generated.h"

class UCgCardData;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(Blueprintable)
struct FCgCardDefinition
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCgCardData> CardData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level;

	bool operator==(const FCgCardDefinition& I) const;
};