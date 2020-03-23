// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/SP_TaskSet.h>

#include <SPlanner/AI/Task/SP_Task.h>

USP_TaskSet::USP_TaskSet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActionClass = USP_Task::StaticClass();
}