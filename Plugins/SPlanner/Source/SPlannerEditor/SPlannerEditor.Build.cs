// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

using UnrealBuildTool;

public class SPlannerEditor : ModuleRules
{
	public SPlannerEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				"CoreUObject",

				"SPlanner"
			}
		);
	}
}