#include <SPlanner/AI/Tasks/Impl/SP_MoveToPOITask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Target/SP_TargetComponent.h>

bool USP_MoveToPOITask::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	// Dirty target will be a POI.
	if (SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPOI))
		return true;

	// No traget change, check current is POI.
	return Cast<USP_AIPlannerComponent>(&Planner)->Target->GetState() == ESP_TargetState::TS_POI;
}