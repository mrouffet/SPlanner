// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SPlannerDemoEditorTarget : TargetRules
{
	public SPlannerDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "SPlannerDemo" } );
	}
}
