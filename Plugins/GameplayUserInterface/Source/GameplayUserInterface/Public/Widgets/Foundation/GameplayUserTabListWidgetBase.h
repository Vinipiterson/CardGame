// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonTabListWidgetBase.h"
#include "GameplayUserTabListWidgetBase.generated.h"

class UGameplayUserButtonBase;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	void RequestRegisterTab(const FName& TabID, const FText& TabDisplayName);
	
	//~Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~End UWidget Interface
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TabList", meta=(AllowPrivateAccess = true, ClampMin = 1, ClampMax = 10))
	int32 DebugEditorPreviewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TabList", meta=(AllowPrivateAccess = true))
	TSubclassOf<UGameplayUserButtonBase> TabButtonEntryWidgetClass;
};
