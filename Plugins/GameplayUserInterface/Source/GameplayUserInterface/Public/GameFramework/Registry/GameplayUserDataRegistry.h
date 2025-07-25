// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "Engine/LocalPlayer.h"
#include "GameplayUserDataRegistry.generated.h"

class UGameplayUserListData;
class UGameplayUserListData_Collection;

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	UGameplayUserDataRegistry();

	// Gets called by options screen right after the object type of UGameplayUserDataRegistry is created.
	virtual void InitRegistry(ULocalPlayer* LocalPlayer);

	FORCEINLINE const TArray<UGameplayUserListData_Collection*>& GetRegisteredCollectionTabs() const { return RegisteredCollectionTabs; }
	TArray<UGameplayUserListData*> GetListSourceItemsBySelectedTabID(const FName& TabID) const;

private:
	UPROPERTY(Transient)
	TArray<UGameplayUserListData_Collection*> RegisteredCollectionTabs;
	
private:
	void FindChildListDataRecursively(UGameplayUserListData* InParentData, TArray<UGameplayUserListData*>& OutFoundChildListData) const;
	
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab(ULocalPlayer* LocalPlayer);
};
