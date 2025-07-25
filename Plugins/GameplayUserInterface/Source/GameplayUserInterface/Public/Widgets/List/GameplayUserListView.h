// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonListView.h"
#include "GameplayUserListView.generated.h"

class UGameplayUserListEntryData;

UCLASS(BlueprintType, Blueprintable)
class GAMEPLAYUSERINTERFACE_API UGameplayUserListView : public UCommonListView
{
	GENERATED_BODY()

public:
	UGameplayUserListView(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	
protected:
	//~Begin UCommonListView interface
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	//~End of UCommonListView interface
	
private:
	UPROPERTY(EditAnywhere, Category = "ListView")
	TObjectPtr<UGameplayUserListEntryData> ListEntryData;
};
