// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "Types/GameplayUserSharedTypes.h"
#include "GameplayUserActivatableWidget.generated.h"

struct FUIInputConfig;
struct FUIActionBindingHandle;

class AGameplayUserPlayerController;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInputActionExecutedSignature, FName, ActionName);

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class GAMEPLAYUSERINTERFACE_API UGameplayUserActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UGameplayUserActivatableWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

	//~Begin UCommonActivatableWidget interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif

	UFUNCTION(BlueprintCallable, Category = "Input")
	void RegisterBinding(FDataTableRowHandle InputAction, const FOnInputActionExecutedSignature& Callback, FUIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void UnregisterBinding(FUIActionBindingHandle BindingHandle);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void UnregisterAllBindings();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	EActivatableInputMode InputMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (EditCondition = "InputMode != EActivatableInputMode::Menu", EditConditionHides))
	EMouseCaptureMode MouseCaptureMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (EditCondition = "InputMode != EActivatableInputMode::Menu", EditConditionHides))
	bool bHidesCursorDuringViewportCapture;

protected:
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Player")
	AGameplayUserPlayerController* GetOwningGameplayUserPlayer();

	TWeakObjectPtr<AGameplayUserPlayerController> OwningGameplayUserPC;
	
	TArray<FUIActionBindingHandle> BindingHandles;
};
