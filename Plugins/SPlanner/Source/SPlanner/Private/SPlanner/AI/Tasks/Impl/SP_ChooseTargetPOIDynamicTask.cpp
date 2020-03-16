// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetPOIDynamicTask.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_ChooseTargetPOIDynamicTask::USP_ChooseTargetPOIDynamicTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Default dimensions > 0.0f.
	MaxDimensions = FVector(100.0f, 100.0f, 100.0f);
}

bool USP_ChooseTargetPOIDynamicTask::PredicatePOI_Implementation(const USP_AIPlannerComponent* Planner, const USP_POIComponent* POI) const
{
	SP_RCHECK_NULLPTR(POI, false)

	if (!Predicate(Planner, POI->GetOwner()))
		return false;

	UClass* POIActorClass = POI->GetOwner()->GetClass();

	return bIgnorePOIActorType ? POIActorTypes.Find(POIActorClass) == INDEX_NONE : POIActorTypes.Find(POIActorClass) != INDEX_NONE;
}

ESP_PlanExecutionState USP_ChooseTargetPOIDynamicTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	APawn* const Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, ESP_PlanExecutionState::PES_Failed)

	FCollisionQueryParams QParams;
	QParams.AddIgnoredActor(Pawn);

	FVector FOVCenter = GetFOVCenter(Pawn);

	TArray<FHitResult> HitInfos;
	Planner->GetWorld()->SweepMultiByChannel(
		HitInfos,
		FOVCenter,
		FOVCenter + FVector(0.0f, 0.0f, 0.1f),
		Pawn->GetActorRotation().Quaternion(),
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeBox(MaxDimensions / 2.0f),
		QParams
	);


	// Find Available POI actors.
	TArray<AActor*> AvailablePOIActors;

	for (int i = 0; i < HitInfos.Num(); ++i)
	{
		USP_POIComponent* const POI = Cast<USP_POIComponent>(HitInfos[i].Component);

		if (POI && PredicatePOI(Planner, POI))
			AvailablePOIActors.Add(POI->GetOwner());
	}

	// No available POI actors.
	if (AvailablePOIActors.Num() == 0)
		return ESP_PlanExecutionState::PES_Failed;

	AActor* const TargetActor = Choose(Planner, AvailablePOIActors);

	// No available target found
	if (!TargetActor)
		return ESP_PlanExecutionState::PES_Failed;

	USP_POIComponent* const POI = Cast<USP_POIComponent>(TargetActor->GetComponentByClass(USP_POIComponent::StaticClass()));

	SetPOITarget(Planner, POI);


#if SP_DEBUG_EDITOR

	DrawDebug(Planner, POI->GetComponentLocation());

	SP_LOG_TASK_EXECUTE(Planner, "%s", *POI->GetOwner()->GetName())

#endif

	return ESP_PlanExecutionState::PES_Succeed;
}