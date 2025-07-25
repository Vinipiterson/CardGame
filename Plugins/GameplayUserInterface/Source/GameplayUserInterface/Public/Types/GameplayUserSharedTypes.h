// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "DeveloperSettings/DeveloperSettings_UserInterface.h"
#include "Internationalization/StringTable.h"
#include "GameplayUserSharedTypes.generated.h"

class UDeveloperSettings_UserInterface;

UENUM(BlueprintType)
enum class EActivatableInputMode : uint8
{
	Menu					UMETA(DisplayName = "Menu"),
	Game					UMETA(DisplayName = "Game"),
	GameAndMenu				UMETA(DisplayName = "Game and Menu"),
	MAX						UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmationDialogType : uint8
{
	Ok						UMETA(DisplayName = "Ok"),
	YesNo					UMETA(DisplayName = "Yes/No"),
	OkCancel				UMETA(DisplayName = "Ok/Cancel"),
	Unknown					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmationDialogResult : uint8
{
	Confirmed				UMETA(DisplayName = "Confirmed"),
	Cancelled				UMETA(DisplayName = "Cancelled"),
	Closed					UMETA(DisplayName = "Closed"),
	Unknown					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified		UMETA(DisplayName = "Directly Modified"),
	DependencyModified		UMETA(DisplayName = "Dependency Modified"),
	ResetToDefault			UMETA(DisplayName = "Reset to Default")
};

UENUM(BlueprintType)
enum class EWidgetActiveState : uint8
{
	Deactivated				UMETA(DisplayName = "Deactivated"),
	Activated				UMETA(DisplayName = "Activated")
};

UENUM(BlueprintType)
enum class EActivatableWidgetAnimType : uint8
{
	None					UMETA(DisplayName = "None"),
	Fade					UMETA(DisplayName = "Fade")
};

USTRUCT(BlueprintType)
struct FConfirmationDialogInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	EConfirmationDialogResult DialogResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	FText ButtonTextToDisplay;

	FConfirmationDialogInfo()
		: DialogResult(EConfirmationDialogResult::Unknown)
		, ButtonTextToDisplay(FText::GetEmpty())
	{
	}
	FConfirmationDialogInfo(EConfirmationDialogResult InDialogResult, FText InButtonTextToDisplay)
		: DialogResult(InDialogResult)
		, ButtonTextToDisplay(InButtonTextToDisplay)
	{
	}
};

USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunc(TFunction<bool()> InEditConditionFunc)
	{
		EditConditionFunc = InEditConditionFunc;
	}

	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}
		
		return true;
	}

	FString GetDisabledRichReason() const
	{
		return DisableRichReason;
	}

	void SetDisableRichReason(const FString& Reason)
	{
		DisableRichReason = Reason;
	}

	bool HasDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

	FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetDisabledForcedStringValue(const FString& ForcedValue)
	{
		DisabledForcedStringValue = ForcedValue;
	}
	
private:
	TFunction<bool()> EditConditionFunc;
	
	FString DisableRichReason;

	TOptional<FString> DisabledForcedStringValue;
};
