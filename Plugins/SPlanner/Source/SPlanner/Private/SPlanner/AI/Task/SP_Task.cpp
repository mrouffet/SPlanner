// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/SP_Task.h>

#include <SPlanner/AI/Planner/SP_AIFloatParam.h>

USP_Task::USP_Task(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<USP_AIFloatParam>(TEXT("Weight")))
{
}