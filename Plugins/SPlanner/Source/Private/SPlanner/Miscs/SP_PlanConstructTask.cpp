#include <SPlanner/Miscs/SP_PlanConstructTask.h>

#include <SPlanner/Debug/SP_Debug.h>
#include <SPlanner/Components/Planners/SP_PlannerComponent.h>

FSP_PlanConstructTask::FSP_PlanConstructTask(USP_PlannerComponent* _Planner) : Planner{ _Planner }
{
}

void FSP_PlanConstructTask::DoWork()
{
	SP_SCHECK_NULLPTR(Planner)

	Planner->ConstructPlan();
}