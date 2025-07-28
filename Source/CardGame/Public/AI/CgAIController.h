// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "CgAIController.generated.h"

class UStateTreeAIComponent;

UCLASS()
class CARDGAME_API ACgAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACgAIController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy")
	TObjectPtr<UStateTreeAIComponent> StateTree;
};
