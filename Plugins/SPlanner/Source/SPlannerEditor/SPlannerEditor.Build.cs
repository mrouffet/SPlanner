using UnrealBuildTool;

public class SPlannerEditor : ModuleRules
{
	public SPlannerEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { });


		PrivateIncludePaths.AddRange(new string[] { });


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"SPlanner"
			}
		);


		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}