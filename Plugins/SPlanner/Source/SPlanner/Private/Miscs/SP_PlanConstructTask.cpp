#include <Miscs/SP_PlanConstructTask.h>

#include <Debug/SP_Debug.h>
#include <Components/Planners/SP_PlannerComponent.h>

FSP_PlanConstructTask::FSP_PlanConstructTask(USP_PlannerComponent* _Planner) : Planner{ _Planner }
{
}

void FSP_PlanConstructTask::DoWork()
{
	SP_CHECK_NULLPTR(Planner)

	Planner->ConstructPlan();
}