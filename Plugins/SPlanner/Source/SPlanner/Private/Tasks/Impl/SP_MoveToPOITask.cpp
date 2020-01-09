#include <Tasks/Impl/SP_MoveToPOITask.h>

#include <Components/SP_PlannerComponent.h>
#include <Components/SP_TargetComponent.h>

bool UMoveToPOITask::PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_TASK_SUPER_PRECONDITION(Planner, PlannerFlags)

	// Dirty target will be a POI.
	if (SP_IS_FLAG_SET(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_POI))
		return true;

	// No traget change, check current is POI.
	return Planner->Target->GetState() == ESP_TargetState::TS_POI;
}