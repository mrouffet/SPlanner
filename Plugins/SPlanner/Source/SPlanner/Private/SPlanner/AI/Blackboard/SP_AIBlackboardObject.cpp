// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/SP_AIBlackboardObject.h>

#include <SPlanner/Debug/SP_Debug.h>

void USP_AIBlackboardObject::Reset(USP_AIBlackboardObject* OriginalObject)
{
	SP_LOG(Warning, "Method must be overridden in children!")
}