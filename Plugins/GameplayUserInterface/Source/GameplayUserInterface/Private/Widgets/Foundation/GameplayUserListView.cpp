// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/List/GameplayUserListView.h"
#include "Widgets/List/GameplayUserListEntryWidget.h"
#include "GameFramework/List/GameplayUserListData_Collection.h"
#include "GameFramework/List/GameplayUserListEntryData.h"
#include "Editor/WidgetCompilerLog.h"

UGameplayUserListView::UGameplayUserListView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

#if WITH_EDITOR
void UGameplayUserListView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!ListEntryData)
	{
		CompileLog.Error(FText::FromString(TEXT("The variable ListEntryData has no valid Data Asset assigned. ") + GetClass()->GetName() + TEXT(" needs a valid Data Asset to function properly.")));
	}
}
#endif

UUserWidget& UGameplayUserListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	if (TSubclassOf<UGameplayUserListEntryWidget> FoundWidgetClass = ListEntryData->FindEntryWidgetClassByListData(CastChecked<UGameplayUserListData>(Item)))
	{
		return GenerateTypedEntry<UGameplayUserListEntryWidget>(FoundWidgetClass, OwnerTable);
	}

	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

bool UGameplayUserListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return !FirstSelectedItem->IsA<UGameplayUserListData_Collection>();
}