// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SPlannerEditor : ModuleRules
{
	public SPlannerEditor(ReadOnlyTargetRules Target) : base(Target)
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
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",

				"UnrealEd",

				"SPlanner"
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}
