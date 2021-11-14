// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SPlannerDemo : ModuleRules
{
	public SPlannerDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",

				"Core",
				"CoreUObject",
		});
	}
}
