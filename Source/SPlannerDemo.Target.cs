// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SPlannerDemoTarget : TargetRules
{
	public SPlannerDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "SPlannerDemo" } );
	}
}
