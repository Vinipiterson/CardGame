// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameplayUserListData.h"
#include "GameplayUserListData_Value.generated.h"

class FGameplayUserOptionsDataAccessor;

UCLASS(Abstract)
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_Value : public UGameplayUserListData
{
	GENERATED_BODY()

public:
	void SetDynamicGetter(const TSharedPtr<FGameplayUserOptionsDataAccessor>& InDynamicGetter);
	void SetDynamicSetter(const TSharedPtr<FGameplayUserOptionsDataAccessor>& InDynamicSetter);

	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; }

	//~Begin UGameplayUserListData interface
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }
	//~End of UGameplayUserListData interface

protected:
	TSharedPtr<FGameplayUserOptionsDataAccessor> DataDynamicGetter;
	TSharedPtr<FGameplayUserOptionsDataAccessor> DataDynamicSetter;

protected:
	FString GetDefaultValueAsString() const { return DefaultStringValue.GetValue(); }

private:
	TOptional<FString> DefaultStringValue;
};
