// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SPlanner : ModuleRules
{
	public SPlanner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[]
			{
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[]
			{
				"SPlanner/Public/SP"
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				"CoreUObject",

				//"AIModule",
				//"NavigationSystem",
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}
