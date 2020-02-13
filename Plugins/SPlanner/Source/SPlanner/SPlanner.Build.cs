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

				"SPlannerEditor"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				"CoreUObject",

				"AIModule",
			}
		);
	}
}