// Copyright Spike Plugins 2025. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "Widgets/Foundation/GameplayUserPrimaryLayoutWidget.h"
#include "GameplayUserPolicy.generated.h"

UCLASS(BlueprintType, Blueprintable, Within = GameplayUserSubsystem)
class GAMEPLAYUSERINTERFACE_API UGameplayUserPolicy : public UObject
{
	GENERATED_BODY()

public:
	UGameplayUserPolicy(const FObjectInitializer& ObjectInitializer);

	//~Begin UObject interface
	virtual UWorld* GetWorld() const override;
	//~End of UObject interface

	template <typename GameplayUserInterfacePolicyClass = UGameplayUserPolicy>
	static GameplayUserInterfacePolicyClass* GetGameplayUserPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<GameplayUserInterfacePolicyClass>(GetGameplayUserPolicy(WorldContextObject));
	}

	static UGameplayUserPolicy* GetGameplayUserPolicy(const UObject* WorldContextObject);
	UGameplayUserSubsystem* GetOwningGameplayUserInterfaceSubsystem() const;

	virtual void InitializePolicy();
	virtual void UninitializePolicy();

	UFUNCTION(BlueprintPure, Category = "Layout")
	UGameplayUserPrimaryLayoutWidget* GetPrimaryLayout() const;

	UFUNCTION(BlueprintPure, Category = "Layout")
	TSubclassOf<UGameplayUserPrimaryLayoutWidget> GetPrimaryLayoutClass() const;

	UFUNCTION(BlueprintPure, Category = "Animation")
	FORCEINLINE EActivatableWidgetAnimType GetActivatableWidgetAnimationType() const { return AnimationType; }
	
	UFUNCTION(BlueprintPure, Category = "Animation")
	FORCEINLINE float GetFadeDuration() const { return FadeDuration; }

protected:
	UPROPERTY(EditAnywhere, Category = "Layout")
	TSubclassOf<UGameplayUserPrimaryLayoutWidget> PrimaryLayoutClass;
	UPROPERTY()
	TObjectPtr<UGameplayUserPrimaryLayoutWidget> PrimaryLayout;
	
protected:
	UFUNCTION(BlueprintCallable)
	void CreateLayoutWidget(APlayerController* Controller);

private:
	UPROPERTY(EditAnywhere, Category = "Animation")
	EActivatableWidgetAnimType AnimationType;
	
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (EditCondition = "AnimationType == EActivatableWidgetAnimType::Fade", EditConditionHides))
	float FadeDuration;
};
