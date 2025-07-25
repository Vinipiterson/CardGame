// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameFramework/List/GameplayUserListData_StringResolution.h"
#include "GameFramework/GameplayUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameplayUserListData_StringResolution, Log, All);

void UGameplayUserListData_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort([](const FIntPoint& A, const FIntPoint& B)->bool
	{
		return A.SizeSquared() < B.SizeSquared();
	});
	
	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResolutionToValueString(Resolution), ResolutionToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResolutionToValueString(AvailableResolutions.Last());
	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UGameplayUserListData_StringResolution::OnDataInitialized()
{
	Super::OnDataInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResolutionToDisplayText(UGameplayUserSettings::Get()->GetScreenResolution());
	}
}

FString UGameplayUserListData_StringResolution::ResolutionToValueString(const FIntPoint& InResolution) const
{
	// Resolution Value from Dynamic Getter: (X=2560,Y=1440).
	return FString::Printf(TEXT("(X=%i,Y=%i)"), InResolution.X, InResolution.Y);
}

FText UGameplayUserListData_StringResolution::ResolutionToDisplayText(const FIntPoint& InResolution) const
{
	// Available Resolution: X=2560 Y=1440.
	const FString DisplayString = FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y);
	return FText::FromString(DisplayString);
}
