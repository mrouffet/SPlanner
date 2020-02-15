// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey.h>

#include <SPlanner/Debug/SP_Debug.h>

void USP_AIBlackboardKey::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	SP_LOG(Error, "Must be overridden in children!")
}
USP_AIBlackboardKey* USP_AIBlackboardKey::CreateInstance(USP_AIBlackboardComponent* Outer)
{
	SP_LOG(Error, "Must be overridden in children!")

	return nullptr;
}