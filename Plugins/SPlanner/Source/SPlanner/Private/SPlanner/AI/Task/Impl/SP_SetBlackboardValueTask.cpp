// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/SP_SetBlackboardValueTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/Misc/VariableParam/Bool/SP_BoolParam.h>
#include <SPlanner/Misc/VariableParam/Float/SP_FloatParam.h>

ESP_PlanExecutionState USP_SetBlackboardValueTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	if (const USP_FloatParam* const FloatParam = Cast<USP_FloatParam>(Param))
		Blackboard->SetFloat(EntryName, FloatParam->Query(Planner));
	else if (const USP_BoolParam* const BoolParam = Cast<USP_BoolParam>(Param))
		Blackboard->SetBool(EntryName, BoolParam->Query(Planner));
	else
	{
		SP_LOG(Error, "Param type not supported yet!")
		return ESP_PlanExecutionState::PES_Failed;
	}

	return ESP_PlanExecutionState::PES_Succeed;
}