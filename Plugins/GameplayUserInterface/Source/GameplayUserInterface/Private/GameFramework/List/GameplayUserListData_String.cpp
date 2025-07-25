// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/List/GameplayUserListData_String.h"
#include "GameplayUserOptionsDataAccessor.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserListData_String, Log, All);

#define LOCTEXT_NAMESPACE "UGameplayUserListData_String"

void UGameplayUserListData_String::OnDataInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}

	if (DataDynamicGetter)
	{
		const FString Result = DataDynamicGetter->GetValueAsString();
		if (!Result.IsEmpty())
		{
			CurrentStringValue = Result;
		}
	}

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
}

void UGameplayUserListData_String::AddDynamicOption(const FString& StringValue, const FText& DisplayText)
{
	AvailableOptionsStringArray.Add(StringValue);
	AvailableOptionsTextArray.Add(DisplayText);
}

void UGameplayUserListData_String::AdvancedToNextOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;
	const bool bIsExistingNextIndex = AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay);

	// For infinite loop
	//CurrentStringValue = bIsExistingNextIndex ? AvailableOptionsStringArray[NextIndexToDisplay] : AvailableOptionsStringArray[0];

	CurrentStringValue = bIsExistingNextIndex ? AvailableOptionsStringArray[NextIndexToDisplay] : AvailableOptionsStringArray.Last();

	TrySetDisplayTextFromStringValue(CurrentStringValue);
	
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		UE_LOG(LogGameplayUserListData_String, Log, TEXT("DataDynamicSetter is used. The latest value from Getter: %s"), *DataDynamicGetter->GetValueAsString());

		NotifyListDataModified(this);
	}
}

void UGameplayUserListData_String::BackToPreviousOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;
	const bool bIsExistingPreviousIndex = AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay);

	// For infinite loop
	//CurrentStringValue = bIsExistingPreviousIndex ? AvailableOptionsStringArray[PreviousIndexToDisplay] : AvailableOptionsStringArray.Last();

	CurrentStringValue = bIsExistingPreviousIndex ? AvailableOptionsStringArray[PreviousIndexToDisplay] : AvailableOptionsStringArray[0];

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		UE_LOG(LogGameplayUserListData_String, Log, TEXT("DataDynamicSetter is used. The latest value from Getter: %s"), *DataDynamicGetter->GetValueAsString());

		NotifyListDataModified(this);
	}
}

void UGameplayUserListData_String::OnRotatorInitiatedValueChanged(const FText& InNewSelectedText)
{
	const int32 FoundIndex = AvailableOptionsTextArray.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool
		{
			return AvailableText.EqualTo(InNewSelectedText);
		}
	);

	if (FoundIndex == INDEX_NONE || !AvailableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		return;
	}

	/*const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;

	const bool bIsExistingPreviousIndex = AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay);
	const bool bIsExistingNextIndex = AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay);*/

	CurrentDisplayText = InNewSelectedText;
	CurrentStringValue = AvailableOptionsStringArray[FoundIndex];

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		NotifyListDataModified(this);
	}
}

bool UGameplayUserListData_String::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UGameplayUserListData_String::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();

		TrySetDisplayTextFromStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

			return true;
		}
	}

	return false;
}

bool UGameplayUserListData_String::CanSetToForcedStringValue(const FString& ForcedValue) const
{
	return CurrentStringValue != ForcedValue;
}

void UGameplayUserListData_String::OnSetToForcedStringValue(const FString& ForcedValue)
{
	CurrentStringValue = ForcedValue;
	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}
}

bool UGameplayUserListData_String::TrySetDisplayTextFromStringValue(const FString& StringValue)
{
	const int32 CurrentFoundIndex = AvailableOptionsStringArray.IndexOfByKey(StringValue);

	if (AvailableOptionsStringArray.IsValidIndex(CurrentFoundIndex))
	{
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIndex];
		return true;
	}

	return false;
}

#undef LOCTEXT_NAMESPACE

//~Begin UGameplayUserListData_StringBool
#define LOCTEXT_NAMESPACE "UGameplayUserListData_StringBool"

void UGameplayUserListData_StringBool::OnDataInitialized()
{
	TryInitBoolValues();

	Super::OnDataInitialized();
}

void UGameplayUserListData_StringBool::OverrideTrueDisplayText(const FText& NewText)
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString, FText::Format(LOCTEXT("UGameplayUserListData_StringBool_TrueString", "{0}"), NewText));
	}
}

void UGameplayUserListData_StringBool::OverrideFalseDisplayText(const FText& NewText)
{
	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, FText::Format(LOCTEXT("UGameplayUserListData_StringBool_FalseString", "{0}"), NewText));
	}
}

void UGameplayUserListData_StringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrueString);
}

void UGameplayUserListData_StringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UGameplayUserListData_StringBool::TryInitBoolValues()
{
	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, LOCTEXT("UGameplayUserListData_StringBool_FalseString", "OFF"));
	}

	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString, LOCTEXT("UGameplayUserListData_StringBool_TrueString", "ON"));
	}
}

#undef LOCTEXT_NAMESPACE

//~Begin UGameplayUserListData_StringEnum
#define LOCTEXT_NAMESPACE "UGameplayUserListData_StringEnum"

void UGameplayUserListData_StringEnum::OnDataInitialized()
{
	Super::OnDataInitialized();
}

#undef LOCTEXT_NAMESPACE

//~Begin UGameplayUserListData_StringInteger
#define LOCTEXT_NAMESPACE "UGameplayUserListData_StringInteger"

void UGameplayUserListData_StringInteger::AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText)
{
	AddDynamicOption(LexToString(InIntegerValue), InDisplayText);
}

void UGameplayUserListData_StringInteger::OnDataInitialized()
{
	Super::OnDataInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = LOCTEXT("CurrentDisplayText_CustomName", "Custom");
	}
}

void UGameplayUserListData_StringInteger::OnEditDependencyModified(UGameplayUserListData* ModifyDependency, EOptionsListDataModifyReason ModifyReason)
{
	if (DataDynamicGetter)
	{
		if (CurrentStringValue == DataDynamicGetter->GetValueAsString())
		{
			return;
		}
		
		CurrentStringValue = DataDynamicGetter->GetValueAsString();

		if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
		{
			CurrentDisplayText = LOCTEXT("CurrentDisplayText_CustomName", "Custom");
		}

		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}

	Super::OnEditDependencyModified(ModifyDependency, ModifyReason);
}

#undef LOCTEXT_NAMESPACE
