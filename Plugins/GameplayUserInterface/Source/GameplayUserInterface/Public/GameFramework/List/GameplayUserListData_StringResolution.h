// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayUserListData_String.h"
#include "GameplayUserListData_StringResolution.generated.h"

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_StringResolution : public UGameplayUserListData_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();

	FORCEINLINE FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }

protected:
	//~Begin UGameplayUserListData_String interface
	virtual void OnDataInitialized() override;
	//~End of UGameplayUserListData_String interface

private:
	FString MaximumAllowedResolution;
	
private:
	FString ResolutionToValueString(const FIntPoint& InResolution) const;
	FText ResolutionToDisplayText(const FIntPoint& InResolution) const;
};
