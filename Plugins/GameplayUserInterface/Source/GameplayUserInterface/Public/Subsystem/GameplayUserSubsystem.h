// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "Types/GameplayUserSharedTypes.h"
#include "GameplayUserSubsystem.generated.h"

class UGameplayUserActivatableWidget;
class UGameplayUserPolicy;
class UGameplayUserButtonBase;

enum class EAsyncPushWidgetState : uint8
{
	BeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedSignature, UGameplayUserButtonBase*, Button, FText, Description);

UCLASS()
class GAMEPLAYUSERINTERFACE_API UGameplayUserSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Gameplay User Subsystem")
	FOnButtonDescriptionTextUpdatedSignature OnButtonDescriptionTextUpdated;

public:
	UGameplayUserSubsystem();
	
	static UGameplayUserSubsystem* Get(const UObject* WorldContextObject);

	//~Begin USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End of USubsystem interface

	void PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag, TSoftClassPtr<UGameplayUserActivatableWidget> WidgetClass, TFunction<void(EAsyncPushWidgetState, UGameplayUserActivatableWidget*)> AsyncPushStateCallback) const;
	void PushConfirmationDialogAsync(EConfirmationDialogType DialogType, const FText& DialogTitle, const FText& DialogMessage, TFunction<void(EConfirmationDialogResult)> OnClicked);
	
	UFUNCTION(BlueprintPure, Category = "Policy")
	UGameplayUserPolicy* GetGameplayUserPolicy() const;

	UFUNCTION(BlueprintCallable, Category="Description")
	void NotifyDescriptionChanged(UGameplayUserButtonBase* Button, FText Description);

protected:
	void SwitchToPolicy(UGameplayUserPolicy* InPolicy);

private:
	UPROPERTY(Transient)
	TObjectPtr<UGameplayUserPolicy> CurrentPolicy;
};
