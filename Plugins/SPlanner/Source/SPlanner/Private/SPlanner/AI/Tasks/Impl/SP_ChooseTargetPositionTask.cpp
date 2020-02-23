// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetPositionTask.h>

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

USP_ChooseTargetPositionTask::USP_ChooseTargetPositionTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Default dimensions > 0.0f.
	HalfDimensions = FVector(100.0f, 100.0f, 100.0f);
}

ESP_PlanExecutionState USP_ChooseTargetPositionTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfosBase* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	const APawn* const Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, ESP_PlanExecutionState::PES_Failed)

	FVector FOVCenter = GetFOVCenter(Pawn);

	// Random position with character's Z (only use XY).
	FVector TargetPosition = FOVCenter + FVector(FMath::RandRange(-1.0f, 1.0f) * HalfDimensions.X, FMath::RandRange(-1.0f, 1.0f) * HalfDimensions.Y, 0.0f);

	// Check visibility.
	if (bTargetVisible)
	{
		FHitResult HitInfos;
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(Pawn);

		if (Planner->GetWorld()->LineTraceSingleByChannel(HitInfos, FOVCenter, TargetPosition, ECollisionChannel::ECC_Visibility, Params))
			TargetPosition = HitInfos.ImpactPoint + HitInfos.Normal; // adjust point.
	}


	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	Target->SetPosition(TargetPosition);

#if SP_DEBUG_EDITOR
	DrawDebug(Planner, TargetPosition);

	SP_LOG_TASK_EXECUTE(Planner, "%s", *TargetPosition.ToString())
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}