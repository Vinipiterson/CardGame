// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/List/GameplayUserListData.h"
#include "GameFramework/GameplayUserSettings.h"

UGameplayUserListData::UGameplayUserListData()
{
	
}

void UGameplayUserListData::InitData()
{
	OnDataInitialized();
}

void UGameplayUserListData::AddEditCondition(const FOptionsDataEditConditionDescriptor& Descriptor)
{
	EditConditionDescriptors.Add(Descriptor);
}

bool UGameplayUserListData::EvaluateEditConditions()
{
	bool bIsEditable = true;

	if (EditConditionDescriptors.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;
	
	for (const FOptionsDataEditConditionDescriptor& Condition : EditConditionDescriptors)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet())
		{
			continue;
		}

		bIsEditable = false;

		CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());

		SetDisabledText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasDisabledForcedStringValue())
 		{
			const FString DisabledForcedStringValue = Condition.GetDisabledForcedStringValue();

			// If the current value this data object has can be set to the forced value.
			if (CanSetToForcedStringValue(DisabledForcedStringValue))
			{
				OnSetToForcedStringValue(DisabledForcedStringValue);
			}
		}
	}

	return bIsEditable;
}

void UGameplayUserListData::AddEditDependency(UGameplayUserListData* Dependency)
{
	if (!Dependency->OnListDataModified.IsBoundToObject(this))
	{
		Dependency->OnListDataModified.AddUObject(this, &UGameplayUserListData::OnEditDependencyModified);
	}
}

void UGameplayUserListData::NotifyListDataModified(UGameplayUserListData* ModifyListData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifyListData, ModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UGameplayUserSettings::Get()->ApplySettings(true);
	}
}

void UGameplayUserListData::OnEditDependencyModified(UGameplayUserListData* ModifyDependency, EOptionsListDataModifyReason ModifyReason)
{
	OnDependencyModified.Broadcast(ModifyDependency, ModifyReason);
}
