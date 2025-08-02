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

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy")
	TObjectPtr<UStateTreeAIComponent> StateTree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy")
	int32 AvoidanceQuality = 4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy")
	float AvoidanceQueryRange = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy")
	float AvoidanceSeparationWeight = 3.f;
};
