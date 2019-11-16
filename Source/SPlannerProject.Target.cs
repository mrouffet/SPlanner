using UnrealBuildTool;
using System.Collections.Generic;

public class SPlannerProjectTarget : TargetRules
{
	public SPlannerProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "SPlannerProject" } );
	}
}
