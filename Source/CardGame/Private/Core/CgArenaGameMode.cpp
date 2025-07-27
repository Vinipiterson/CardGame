// Copyright Vinipi. All Rights Reserved.


#include "Core/CgArenaGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CgPlayerController.h"

AActor* ACgArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	int32 PlayerID = GetNumPlayers() + 1;
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		APlayerStart* Start = Cast<APlayerStart>(Actor);
		if (Start->PlayerStartTag == FString::FromInt(PlayerID))
			return Start;
	}

	return Actors[0];
}

void ACgArenaGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (ACgPlayerController* PC = Cast<ACgPlayerController>(NewPlayer))
	{
		PC->Team = GetNumPlayers();
	}
}
