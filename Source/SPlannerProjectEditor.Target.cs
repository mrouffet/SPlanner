using UnrealBuildTool;
using System.Collections.Generic;

public class SPlannerProjectEditorTarget : TargetRules
{
	public SPlannerProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "SPlannerProject" } );
	}
}
