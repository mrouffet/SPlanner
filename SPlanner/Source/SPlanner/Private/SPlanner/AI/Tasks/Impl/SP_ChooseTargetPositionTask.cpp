// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetPositionTask.h>

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/Objects/Target/SP_Target.h>

USP_ChooseTargetPositionTask::USP_ChooseTargetPositionTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Default dimensions.
	MinLocalExtent = FVector(0.0f, 0.0f, 0.0f);
	MaxLocalExtent = FVector(100.0f, 100.0f, 100.0f);
}

ESP_PlanExecutionState USP_ChooseTargetPositionTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	const APawn* const Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, ESP_PlanExecutionState::PES_Failed)

	const FVector FOVCenter = GetFOVCenter(Pawn);

	// Random position with character's Z (only use XY).
	FVector TargetPosition = FOVCenter;
	FVector FOVMinHalfExtent = GetFOVMinExtent(Pawn) / 2.0f;
	FVector FOVMaxHalfExtent = GetFOVMaxExtent(Pawn) / 2.0f;

	TargetPosition.X += FMath::RandRange(FOVMinHalfExtent.X, FOVMaxHalfExtent.X) * (FMath::RandBool() ? 1.0f : -1.0f);
	TargetPosition.Y += FMath::RandRange(FOVMinHalfExtent.Y, FOVMaxHalfExtent.Y) * (FMath::RandBool() ? 1.0f : -1.0f);
	TargetPosition.Z += FMath::RandRange(FOVMinHalfExtent.Z, FOVMaxHalfExtent.Z) * (FMath::RandBool() ? 1.0f : -1.0f);

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

	USP_Target* const OutTarget = Blackboard->GetObject<USP_Target>(OutTargetEntryName);
	SP_RCHECK_NULLPTR(OutTarget, ESP_PlanExecutionState::PES_Failed)

	OutTarget->SetPosition(TargetPosition);

#if SP_DEBUG_EDITOR
	DrawDebug(Planner, TargetPosition);

	SP_LOG_TASK_EXECUTE(Planner, "%s", *TargetPosition.ToString())
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}