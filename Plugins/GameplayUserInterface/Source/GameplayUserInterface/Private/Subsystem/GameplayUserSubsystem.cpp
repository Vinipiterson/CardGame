// Copyright Spike Plugins 2025. All Rights Reserved.

#include "Subsystem/GameplayUserSubsystem.h"
#include "GameFramework/GameplayUserPolicy.h"
#include "Engine/AssetManager.h"
#include "Library/GameplayUserLibrary.h"
#include "Widgets/Foundation/GameplayUserActivatableWidget.h"
#include "Widgets/Foundation/GameplayUserPrimaryLayoutWidget.h"
#include "Widgets/Dialog/GameplayUserConfirmationDialogWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "DeveloperSettings/DeveloperSettings_UserInterface.h"
#include "GameplayUserTags.h"
#include "Engine/Engine.h"              // for GEngine
#include "Engine/GameInstance.h"        // for UGameInstance
#include "Engine/World.h"               // for GetWorld()
#include "Kismet/GameplayStatics.h"     // for UGameplayStatics
#include "Misc/AssertionMacros.h"       // for check(), ensure()

UGameplayUserSubsystem::UGameplayUserSubsystem()
	: CurrentPolicy(nullptr)
{
	
}

void UGameplayUserSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UDeveloperSettings_UserInterface* UserInterfaceSettings = GetDefault<UDeveloperSettings_UserInterface>();
	if (UserInterfaceSettings)
	{
		TSubclassOf<UGameplayUserPolicy> InterfacePolicyClass = UserInterfaceSettings->GameplayUserPolicyClass.LoadSynchronous();
		if (InterfacePolicyClass)
		{
			SwitchToPolicy(NewObject<UGameplayUserPolicy>(this, InterfacePolicyClass));
		}
	}
}

void UGameplayUserSubsystem::Deinitialize()
{
	Super::Deinitialize();

	SwitchToPolicy(nullptr);
}

UGameplayUserSubsystem* UGameplayUserSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		if (IsValid(World))
		{
			return UGameInstance::GetSubsystem<UGameplayUserSubsystem>(World->GetGameInstance());
		}
	}

	return nullptr;
}

bool UGameplayUserSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UGameplayUserSubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag, TSoftClassPtr<UGameplayUserActivatableWidget> WidgetClass, TFunction<void(EAsyncPushWidgetState, UGameplayUserActivatableWidget*)> AsyncPushStateCallback) const
{
	check(!WidgetClass.IsNull());
	check(CurrentPolicy);

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
		[WidgetStackTag, WidgetClass, AsyncPushStateCallback, this]()
		{
			UClass* LoadedWidgetClass = WidgetClass.Get();
			const UGameplayUserPrimaryLayoutWidget* PrimaryLayout = CurrentPolicy->GetPrimaryLayout();
			
			check(LoadedWidgetClass && PrimaryLayout);

			UCommonActivatableWidgetContainerBase* FoundWidgetStack = PrimaryLayout->FindWidgetStackByTag(WidgetStackTag);
			UGameplayUserActivatableWidget* CreatedWidget = FoundWidgetStack->AddWidget<UGameplayUserActivatableWidget>(LoadedWidgetClass, [FoundWidgetStack, AsyncPushStateCallback](UGameplayUserActivatableWidget& CreatedWidgetInstance)
			{
				AsyncPushStateCallback(EAsyncPushWidgetState::BeforePush, &CreatedWidgetInstance);
			});

			AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
		})
	);
}

void UGameplayUserSubsystem::PushConfirmationDialogAsync(EConfirmationDialogType DialogType, const FText& DialogTitle, const FText& DialogMessage, TFunction<void(EConfirmationDialogResult)> OnClicked)
{
	UGameplayUserConfirmationDialogDescriptor* CreatedDialogDescriptor = nullptr;
	
	switch (DialogType)
	{
	case EConfirmationDialogType::Ok:
		CreatedDialogDescriptor = UGameplayUserConfirmationDialogDescriptor::CreateOkDialog(DialogTitle, DialogMessage);
		break;
	case EConfirmationDialogType::YesNo:
		CreatedDialogDescriptor = UGameplayUserConfirmationDialogDescriptor::CreateYesNoDialog(DialogTitle, DialogMessage);
		break;
	case EConfirmationDialogType::OkCancel:
		CreatedDialogDescriptor = UGameplayUserConfirmationDialogDescriptor::CreateOkCancelDialog(DialogTitle, DialogMessage);
		break;
	case EConfirmationDialogType::Unknown:
		break;
	default:
		break;
	}

	check(CreatedDialogDescriptor);

	PushSoftWidgetToStackAsync(GameplayUserTags::LayerStack::LayerStack_Modal, UGameplayUserLibrary::GetConfirmationDialogWidget(),
	[CreatedDialogDescriptor, OnClicked](EAsyncPushWidgetState InPushState, UGameplayUserActivatableWidget* InWidget)
	{
		if (InPushState == EAsyncPushWidgetState::BeforePush)
		{
			UGameplayUserConfirmationDialogWidget* CreatedConfirmDialog = CastChecked<UGameplayUserConfirmationDialogWidget>(InWidget);
			CreatedConfirmDialog->InitConfirmationDialog(CreatedDialogDescriptor, OnClicked);
		}
	});
}

UGameplayUserPolicy* UGameplayUserSubsystem::GetGameplayUserPolicy() const
{
	return CurrentPolicy;
}

void UGameplayUserSubsystem::NotifyDescriptionChanged(UGameplayUserButtonBase* Button, FText Description)
{
	if (OnButtonDescriptionTextUpdated.IsBound())
	{
		OnButtonDescriptionTextUpdated.Broadcast(Button, Description);
	}
}

void UGameplayUserSubsystem::SwitchToPolicy(UGameplayUserPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		UGameplayUserPolicy* OldPolicy = CurrentPolicy;
		if (OldPolicy)
		{
			OldPolicy->UninitializePolicy();
		}

		CurrentPolicy = InPolicy;
		if (CurrentPolicy)
		{
			CurrentPolicy->InitializePolicy();
		}
	}
}
