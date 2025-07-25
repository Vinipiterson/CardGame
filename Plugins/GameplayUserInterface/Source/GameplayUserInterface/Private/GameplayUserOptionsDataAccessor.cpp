// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameplayUserOptionsDataAccessor.h"
#include "GameFramework/GameplayUserSettings.h"

FGameplayUserOptionsDataAccessor::FGameplayUserOptionsDataAccessor(const FString& InSetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	CachedWeakGameUserSettings = UGameplayUserSettings::Get();
}

FString FGameplayUserOptionsDataAccessor::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(CachedWeakGameUserSettings.Get(), CachedDynamicFunctionPath, OutStringValue);
	return OutStringValue;
}

void FGameplayUserOptionsDataAccessor::SetValueFromString(const FString& InStringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(CachedWeakGameUserSettings.Get(), CachedDynamicFunctionPath, InStringValue);
}
