// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CgPlayerController.generated.h"

UCLASS()
class CARDGAME_API ACgPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACgPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 Team = -1;
};
