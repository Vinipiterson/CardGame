// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/GameplayUserPolicy.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/Foundation/GameplayUserPrimaryLayoutWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Subsystem/GameplayUserSubsystem.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserPolicy, Log, All);

UGameplayUserPolicy::UGameplayUserPolicy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UGameplayUserPrimaryLayoutWidget> DefaultPrimaryLayoutClass(TEXT("/GameplayUserInterface/Blueprints/Widgets/Foundation/WBP_GUI_PrimaryLayout"));
	if (DefaultPrimaryLayoutClass.Succeeded())
	{
		PrimaryLayoutClass = DefaultPrimaryLayoutClass.Class;
	}

	AnimationType = EActivatableWidgetAnimType::Fade;
	FadeDuration = 2.0f;
}

void UGameplayUserPolicy::InitializePolicy()
{
	if (!PrimaryLayoutClass)
	{
		UE_LOG(LogGameplayUserPolicy, Error, TEXT("PrimaryLayoutClass is not valid."));
		return;
	}
}

void UGameplayUserPolicy::UninitializePolicy()
{
	if (PrimaryLayout)
	{
		PrimaryLayout->RemoveFromParent();
		PrimaryLayout = nullptr;
	}
}

UWorld* UGameplayUserPolicy::GetWorld() const
{
	return GetOwningGameplayUserInterfaceSubsystem()->GetGameInstance()->GetWorld();
}

UGameplayUserPolicy* UGameplayUserPolicy::GetGameplayUserPolicy(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return nullptr;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance) return nullptr;

	UGameplayUserSubsystem* GameplayUserSubsystem = UGameInstance::GetSubsystem<UGameplayUserSubsystem>(GameInstance);
	if (!GameplayUserSubsystem) return nullptr;

	return GameplayUserSubsystem->GetGameplayUserPolicy();
}

UGameplayUserSubsystem* UGameplayUserPolicy::GetOwningGameplayUserInterfaceSubsystem() const
{
	return CastChecked<UGameplayUserSubsystem>(GetOuter());
}

UGameplayUserPrimaryLayoutWidget* UGameplayUserPolicy::GetPrimaryLayout() const
{	
	return PrimaryLayout;
}

TSubclassOf<UGameplayUserPrimaryLayoutWidget> UGameplayUserPolicy::GetPrimaryLayoutClass() const
{
	return PrimaryLayoutClass;
}

void UGameplayUserPolicy::CreateLayoutWidget(APlayerController* Controller)
{
	if (!IsValid(Controller)) return;

	PrimaryLayout = CreateWidget<UGameplayUserPrimaryLayoutWidget>(Controller, PrimaryLayoutClass);
	if (!IsValid(PrimaryLayout)) return;
	
	PrimaryLayout->SetPlayerContext(FLocalPlayerContext(Controller));
	PrimaryLayout->AddToPlayerScreen(1000);
	
	UE_LOG(LogGameplayUserPolicy, Log, TEXT("PrimaryLayout has been created and configured."));
}