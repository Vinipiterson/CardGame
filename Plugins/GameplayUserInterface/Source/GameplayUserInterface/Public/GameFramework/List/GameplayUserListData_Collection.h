// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayUserListData.h"
#include "GameplayUserListData_Collection.generated.h"

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_Collection : public UGameplayUserListData
{
	GENERATED_BODY()

public:
	UGameplayUserListData_Collection();

	//~Begin UGameplayUserListData interface
	virtual TArray<UGameplayUserListData*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;
	//~End of UGameplayUserListData interface

	void AddChildListData(UGameplayUserListData* ChildListData);

private:
	UPROPERTY(Transient)
	TArray<UGameplayUserListData*> ChildListDataArray;
};
