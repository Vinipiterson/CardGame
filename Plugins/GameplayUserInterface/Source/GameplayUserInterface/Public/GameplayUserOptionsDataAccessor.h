// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UGameplayUserSettings;

// This is the full code for constructor data interaction helper.
//TSharedPtr<FGameplayUserOptionsDataAccessor> ConstructedHelper = MakeShared<FGameplayUserOptionsDataAccessor>(GET_FUNCTION_NAME_STRING_CHECKED(UGameplayUserSettings, GetGameDifficulty));

#define OPTIONS_ACCESSOR(SetterOrGetterFuncName) \
	MakeShared<FGameplayUserOptionsDataAccessor>(GET_FUNCTION_NAME_STRING_CHECKED(UGameplayUserSettings, SetterOrGetterFuncName)) \

class GAMEPLAYUSERINTERFACE_API FGameplayUserOptionsDataAccessor
{
public:
	FGameplayUserOptionsDataAccessor(const FString& InSetterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& InStringValue);

private:
	FCachedPropertyPath CachedDynamicFunctionPath;
	TWeakObjectPtr<UGameplayUserSettings> CachedWeakGameUserSettings;
};
