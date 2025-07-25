// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "GameFramework/List/GameplayUserListData_Value.h"
#include "GameplayUserListData_String.generated.h"

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_String : public UGameplayUserListData_Value
{
	GENERATED_BODY()

public:
	void AddDynamicOption(const FString& StringValue, const FText& DisplayText);
	
	void AdvancedToNextOption();
	void BackToPreviousOption();

	void OnRotatorInitiatedValueChanged(const FText& InNewSelectedText);

	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }
	FORCEINLINE FString GetCurrentStringValue() const { return CurrentStringValue; }
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FORCEINLINE const TArray<FString>& GetAvailableOptionsStringArray() const { return AvailableOptionsStringArray; }
	FORCEINLINE int32 GetCurrentStringValueIndex() const { return AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue); }
	FORCEINLINE int32 GetNumStringValueOptions() const { return AvailableOptionsStringArray.Num(); }
	
protected:
	FString CurrentStringValue;
	FText CurrentDisplayText;
	
	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;

protected:
	//~Begin UGameplayUserListData interface
	virtual void OnDataInitialized() override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	virtual bool CanSetToForcedStringValue(const FString& ForcedValue) const override;
	virtual void OnSetToForcedStringValue(const FString& ForcedValue) override;
	//~End of UGameplayUserListData interface

	bool TrySetDisplayTextFromStringValue(const FString& StringValue);
};

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_StringBool : public UGameplayUserListData_String
{
	GENERATED_BODY()

public:
	void OverrideTrueDisplayText(const FText& NewText);
	void OverrideFalseDisplayText(const FText& NewText);

	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();
	
protected:
	//~Begin UGameplayUserListData interface
	virtual void OnDataInitialized() override;
	//~End of UGameplayUserListData interface

private:
	void TryInitBoolValues();
	
	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");
};

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_StringEnum : public UGameplayUserListData_String
{
	GENERATED_BODY()

public:
	template <typename EnumType>
	void AddEnumOption(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		AddDynamicOption(ConvertedEnumString, InDisplayText);
	}

	template <typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		return (EnumType)StaticEnumOption->GetValueByNameString(CurrentStringValue);
	}

	template <typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		SetDefaultValueFromString(ConvertedEnumString);
	}
	
protected:
	//~Begin UGameplayUserListData interface
	virtual void OnDataInitialized() override;
	//~End of UGameplayUserListData interface
};

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData_StringInteger : public UGameplayUserListData_String
{
	GENERATED_BODY()

public:
	void AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText);

protected:
	//~Begin UGameplayUserListData interface
	virtual void OnDataInitialized() override;
	virtual void OnEditDependencyModified(UGameplayUserListData* ModifyDependency, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified) override;
	//~End of UGameplayUserListData interface
};
