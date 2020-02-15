// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey.h>

#include <SPlanner/Debug/SP_Debug.h>

void USP_AIBlackboardKey::CopyValue(const USP_AIBlackboardKey* Other)
{
	SP_LOG(Error, "Must be overridden in children!")
}