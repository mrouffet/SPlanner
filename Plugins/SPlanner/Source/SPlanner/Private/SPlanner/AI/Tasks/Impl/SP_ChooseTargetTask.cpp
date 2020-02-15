// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Tasks/Impl/SP_LookAtTask.h>

uint32 USP_ChooseTargetTask::GetUserDataSize() const
{
	return sizeof(ESP_InternalTickState) + (LookAtTask ? LookAtTask->GetUserDataSize() : 0);
}

bool USP_ChooseTargetTask::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

#if SP_DEBUG
	// Check valid blackboard entry.
	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)
#endif

	// Not already re-targeted.
	if(bAllowRetarget)
		return !SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty);

	// Try to valid LookAt.
	if (LookAtTask)
		return LookAtTask->PreCondition(Planner, GeneratedPlan, PostCondition(Planner, PlannerFlags));

	return true;
}
uint64 USP_ChooseTargetTask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPosition);

	// Add LookAt flags.
	if (LookAtTask)
		PlannerFlags = LookAtTask->PostCondition(Planner, PlannerFlags);

	return PlannerFlags;
}

bool USP_ChooseTargetTask::Begin(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_BEGIN_SUPER(Planner, UserData)

	// Set internal state.
	if (LookAtTask)
		ESP_InternalTickState* InternalTickState = new(UserData) ESP_InternalTickState{ ESP_InternalTickState::ITS_WaitForSuccess };

	return true;
}
ESP_PlanExecutionState USP_ChooseTargetTask::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_TICK_SUPER(DeltaSeconds, Planner, UserData)

	// No lookat next: only internal implementation.
	if(!LookAtTask)
		return Tick_Internal(DeltaSeconds, Planner, UserData);

	ESP_PlanExecutionState InternalPlanState = Tick_Internal(DeltaSeconds, Planner, UserData);

	// Wait for success.
	if(InternalPlanState != ESP_PlanExecutionState::PES_Succeed)
		return InternalPlanState;

	ESP_InternalTickState* InternalTickState = reinterpret_cast<ESP_InternalTickState*>(UserData);

	// Begin LookAt.
	if (*InternalTickState == ESP_InternalTickState::ITS_WaitForSuccess)
	{
		LookAtTask->Begin(Planner, UserData + sizeof(ESP_InternalTickState));
		*InternalTickState = ESP_InternalTickState::ITS_Tick;
	}

	ESP_PlanExecutionState LookAtState = LookAtTask->Tick(DeltaSeconds, Planner, UserData + sizeof(ESP_PlanExecutionState));

	// Wait for success.
	if(LookAtState != ESP_PlanExecutionState::PES_Succeed)
		return LookAtState;

	LookAtTask->End(Planner, UserData + sizeof(ESP_InternalTickState));

	return ESP_PlanExecutionState::PES_Succeed;
}
bool USP_ChooseTargetTask::End(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_END_SUPER(Planner, UserData)

	if (LookAtTask)
		reinterpret_cast<ESP_InternalTickState*>(UserData)->~ESP_InternalTickState();

	return true;
}

bool USP_ChooseTargetTask::Cancel(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_CANCEL_SUPER(Planner, UserData)

	// Look at has started.
	if (LookAtTask && *reinterpret_cast<ESP_InternalTickState*>(UserData) == ESP_InternalTickState::ITS_Tick)
		LookAtTask->Cancel(Planner, UserData);

	return true;
}

ESP_PlanExecutionState USP_ChooseTargetTask::Tick_Internal(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	// Overridden in children.
	return ESP_PlanExecutionState::PES_Succeed;
}