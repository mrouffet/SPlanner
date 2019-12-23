#include <Miscs/PlanConstructTask.h>

#include <Debug/Debug.h>
#include <Components/PlannerComponent.h>

FSP_PlanConstructTask::FSP_PlanConstructTask(USP_PlannerComponent* _Planner) : Planner{ _Planner }
{
}

void FSP_PlanConstructTask::DoWork()
{
	SP_CHECK_NULLPTR(Planner)

	Planner->ConstructPlan();
}