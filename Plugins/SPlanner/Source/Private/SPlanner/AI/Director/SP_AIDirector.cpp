#include <SPlanner/AI/Director/SP_AIDirector.h>

void ASP_AIDirector::OnRegistedPlannerActive(USP_PlannerComponent* InPlanner)
{
	Super::OnRegistedPlannerActive(InPlanner);

	OnAINumChange.Broadcast(GetPlannerNum());
}
void ASP_AIDirector::OnRegistedPlannerInactive(USP_PlannerComponent* InPlanner)
{
	Super::OnRegistedPlannerInactive(InPlanner);

	OnAINumChange.Broadcast(GetPlannerNum());
}