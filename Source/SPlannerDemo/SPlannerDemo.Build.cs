// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

using UnrealBuildTool;

public class SPlannerDemo : ModuleRules
{
	public SPlannerDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
