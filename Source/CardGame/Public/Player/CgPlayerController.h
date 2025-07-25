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

protected:
	virtual void BeginPlay() override;

	//~Camera

	// 1 = First player, 2 = second player
	UFUNCTION(BlueprintImplementableEvent)
	AActor* GetViewTargetForPlayer(int32 PlayerID);
	//~Camera
};
