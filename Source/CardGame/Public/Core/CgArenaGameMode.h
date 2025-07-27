// Copyright Vinipi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CgArenaGameMode.generated.h"

UCLASS()
class CARDGAME_API ACgArenaGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
};
