// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Widgets/Foundation/GameplayUserTabListWidgetBase.h"
#include "Widgets/Foundation/GameplayUserButtonBase.h"
#include "Editor/WidgetCompilerLog.h"

void UGameplayUserTabListWidgetBase::RequestRegisterTab(const FName& TabID, const FText& TabDisplayName)
{
	RegisterTab(TabID, TabButtonEntryWidgetClass, nullptr);

	if (UGameplayUserButtonBase* FoundButton = Cast<UGameplayUserButtonBase>(GetTabButtonBaseByID(TabID)))
	{
		FoundButton->SetButtonText(TabDisplayName);
	}
}

#if WITH_EDITOR	
void UGameplayUserTabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified. ") + GetClass()->GetName() + TEXT(" needs a valid entry widget class to function properly.")));
	}
}
#endif
