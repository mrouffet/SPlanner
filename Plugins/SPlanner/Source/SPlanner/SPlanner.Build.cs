using UnrealBuildTool;

public class SPlanner : ModuleRules
{
	public SPlanner(ReadOnlyTargetRules Target) : base(Target)
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
			}
		);


		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}