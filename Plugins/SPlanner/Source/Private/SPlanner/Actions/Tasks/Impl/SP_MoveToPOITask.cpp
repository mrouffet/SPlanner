#include <SPlanner/Actions/Tasks/Impl/SP_MoveToPOITask.h>

#include <SPlanner/Miscs/Flags/SP_AIPlannerFlags.h>

#include <SPlanner/Components/Planners/SP_AIPlannerComponent.h>
#include <SPlanner/Components/SP_TargetComponent.h>

bool UMoveToPOITask::PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, PlannerFlags)

	// Dirty target will be a POI.
	if (SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPOI))
		return true;

	// No traget change, check current is POI.
	return Cast<USP_AIPlannerComponent>(Planner)->Target->GetState() == ESP_TargetState::TS_POI;
}