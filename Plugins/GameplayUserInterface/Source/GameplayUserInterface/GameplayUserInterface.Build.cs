// Copyright Spike Plugins 2025. All Rights Reserved.

using UnrealBuildTool;

public class GameplayUserInterface : ModuleRules
{
	public GameplayUserInterface (ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"InputCore",
                "CoreUObject",
                "Engine",
                "CommonUI",
				"GameplayTags",
				"UMG",
				"CommonInput",
				"EnhancedInput"
            }
		);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"DeveloperSettings",
				"Slate",
				"SlateCore"
			}
		);
		
		if (Target.bBuildEditor) 
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"PropertyPath"
				}
			);
		}
	}
}
