// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetPOIStaticTask.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

void USP_ChooseTargetPOIStaticTask::Register(USP_POIComponent* POI)
{
	SP_CHECK_NULLPTR(POI)
	SP_CHECK(POIs.Find(POI) == INDEX_NONE, "POI [%s] already registered!", *POI->GetOwner()->GetName())

	POIs.Add(POI);
}
void USP_ChooseTargetPOIStaticTask::UnRegister(USP_POIComponent* POI)
{
	SP_CHECK_NULLPTR(POI)
	SP_CHECK(POIs.Find(POI) != INDEX_NONE, "POI [%s] not previously registered!", *POI->GetOwner()->GetName())

	POIs.Remove(POI);
}

ESP_PlanExecutionState USP_ChooseTargetPOIStaticTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	// Find Available POIs.
	TArray<AActor*> AvailablePOIActors;

	for (int i = 0; i < POIs.Num(); ++i)
	{
		SP_CCHECK(POIs[i], " POIs[%d] nullptr!", i)

		if (Predicate(Planner, POIs[i]->GetOwner()))
			AvailablePOIActors.Add(POIs[i]->GetOwner());
	}


	// No available POI actors.
	if (AvailablePOIActors.Num() == 0)
		return ESP_PlanExecutionState::PES_Failed;

	AActor* const TargetActor = Choose(Planner, AvailablePOIActors);

	// No available target found
	if(!TargetActor)
		return ESP_PlanExecutionState::PES_Failed;

	USP_POIComponent* const POI = Cast<USP_POIComponent>(TargetActor->GetComponentByClass(USP_POIComponent::StaticClass()));

	SetPOITarget(Planner, POI);


#if SP_DEBUG_EDITOR

	DrawDebug(Planner, POI->GetOwner()->GetActorLocation());

	SP_LOG_TASK_EXECUTE(Planner, "%s", *POI->GetOwner()->GetName())

#endif

	return ESP_PlanExecutionState::PES_Succeed;
}