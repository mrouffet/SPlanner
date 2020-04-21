// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

using UnrealBuildTool;

public class SPlanner : ModuleRules
{
	public SPlanner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


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

				"AIModule",
				"NavigationSystem",
			}
		);


		// Editor only.
		if (Target.Type == TargetType.Editor)
			PublicDependencyModuleNames.Add("SPlannerEditor");
	}
}