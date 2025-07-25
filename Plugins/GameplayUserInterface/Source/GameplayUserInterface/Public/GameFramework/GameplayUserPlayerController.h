// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "GameplayUserPlayerController.generated.h"

UCLASS()
class GAMEPLAYUSERINTERFACE_API AGameplayUserPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGameplayUserPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay User Interface")
	uint8 bAutoPossesMenuCamera : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay User Interface", meta = (EditCondition = "bAutoPossesMenuCamera"))
	FName MenuCameraTag;

protected:
	//~Begin AController interface
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

	void TryToPossesMenuCamera();

private:
	TArray<AActor*> FoundedMenuCameras;
};
