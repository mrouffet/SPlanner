#include <Tasks/Impl/MoveToPOITask.h>

#include <Components/PlannerComponent.h>
#include <Components/TargetComponent.h>

bool UMoveToPOITask::PreCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const
{
	SP_TASK_SUPER_PRECONDITION(Planner, PlannerFlags)

	return Planner->GetTarget()->GetState() == ESP_TargetState::TS_POI;
}