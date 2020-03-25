// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_Action.h>

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>

USP_Action::USP_Action(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Default Weight.
	Weight = CreateDefaultSubobject<USP_FloatParam>("Weight");
	Weight->DefaultValue = 1.0f;
}