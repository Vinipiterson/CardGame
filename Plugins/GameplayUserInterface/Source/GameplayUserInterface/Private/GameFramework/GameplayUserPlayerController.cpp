// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/GameplayUserPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

AGameplayUserPlayerController::AGameplayUserPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPossesMenuCamera = false;
	MenuCameraTag = FName("MenuCamera");
}

void AGameplayUserPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TryToPossesMenuCamera();
}

void AGameplayUserPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	TryToPossesMenuCamera();
}

void AGameplayUserPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	TryToPossesMenuCamera();
}

void AGameplayUserPlayerController::TryToPossesMenuCamera()
{
	if (!bAutoPossesMenuCamera) return;

	if (FoundedMenuCameras.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), MenuCameraTag, FoundedMenuCameras);
		if (!FoundedMenuCameras.IsEmpty())
		{
			SetViewTarget(FoundedMenuCameras[0]);
		}
	}
}
