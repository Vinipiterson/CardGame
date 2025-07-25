// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "Types/GameplayUserSharedTypes.h"
#include "GameplayUserListData.generated.h"

#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType New##PropertyName) { PropertyName = New##PropertyName; } 

UCLASS(Abstract)
class GAMEPLAYUSERINTERFACE_API UGameplayUserListData : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedSignature, UGameplayUserListData*, EOptionsListDataModifyReason);
	FOnListDataModifiedSignature OnListDataModified;
	FOnListDataModifiedSignature OnDependencyModified;
	
public:
	UGameplayUserListData();

	LIST_DATA_ACCESSOR(FName, DevName)
	LIST_DATA_ACCESSOR(FText, DisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionText)
	LIST_DATA_ACCESSOR(FText, DisabledText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, DescriptionImage)
	LIST_DATA_ACCESSOR(TObjectPtr<UGameplayUserListData>, ParentData)

	// Empty in the base class. Child class UGameplayUserListDataCollection should override it. The function should return all the child data a tab has.
	virtual TArray<UGameplayUserListData*> GetAllChildListData() const { return TArray<UGameplayUserListData*>(); }
	virtual bool HasAnyChildListData() const { return false; }

	void InitData();

	void SetShouldApplySettingImmediately(bool bShouldApplyRightAway) { bShouldApplyChangeImmediately = bShouldApplyRightAway; }

	// The child class should override them to provide implementations for resetting the data.
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }

	// Gets called from OptionsDataRegistry for adding in edit conditions for the constructed list data objects.
	void AddEditCondition(const FOptionsDataEditConditionDescriptor& Descriptor);
	bool EvaluateEditConditions();

	// Gets called from OptionsDataRegistry to add in dependency data.
	void AddEditDependency(UGameplayUserListData* Dependency);

protected:
	// Empty in base class. The child classes should override it to handle the initialized needed accordingly.
	virtual void OnDataInitialized() { };

	virtual void NotifyListDataModified(UGameplayUserListData* ModifyListData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

	// The child class should override this to allow the value be set to the forced string value.
	virtual bool CanSetToForcedStringValue(const FString& ForcedValue) const { return false; }

	// The child class should override this to specify how to set the current value to the forced value.
	virtual void OnSetToForcedStringValue(const FString& ForcedValue) { }

	// This function will be called when the value of the dependency data has changed. The child class can override this function to handle the custom logic needed. Super call is expected.
	virtual void OnEditDependencyModified(UGameplayUserListData* ModifyDependency, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

private:
	FName DevName;
	
	FText DisplayName;
	FText DescriptionText;
	FText DisabledText;

	TSoftObjectPtr<UTexture2D> DescriptionImage;

	bool bShouldApplyChangeImmediately = false;

	UPROPERTY(Transient)
	TObjectPtr<UGameplayUserListData> ParentData;
	
	UPROPERTY(Transient)
	TArray<FOptionsDataEditConditionDescriptor> EditConditionDescriptors;
};
