// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetFleePositionTask.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/Object/Target/SP_Target.h>

USP_ChooseTargetFleePositionTask::USP_ChooseTargetFleePositionTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTargetVisible = true;

	// Default dimensions.
	MinLocalExtent = FVector(0.0f, 0.0f, 0.0f);
	MaxLocalExtent = FVector(100.0f, 100.0f, 100.0f);
}

FVector USP_ChooseTargetFleePositionTask::ComputeFleePoint(const FVector& FleeDirection, const FVector& FOVCenter) const
{
	// Flee point world location bound to max extent.

	FVector FleePoint = FleeDirection;

	// Scale direction by Extent.
	FleePoint.X *= MaxLocalExtent.X / 2.0f;
	FleePoint.Y *= MaxLocalExtent.Y / 2.0f;
	
	// Add FOV location.
	FleePoint += FOVCenter;

	return FleePoint;
}

bool USP_ChooseTargetFleePositionTask::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	// Dirty Flee target.
	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	if(AIPlanGenInfos->IsBlackboardFlagSet(FleeTargetEntryName, ESP_AIBBPlanGenFlags::PG_Dirty))
		return true;

	return true;
}

ESP_PlanExecutionState USP_ChooseTargetFleePositionTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	const APawn* const Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, ESP_PlanExecutionState::PES_Failed)

	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	USP_Target* const OutTarget = Blackboard->GetObject<USP_Target>(OutTargetEntryName);
	SP_RCHECK_NULLPTR(OutTarget, ESP_PlanExecutionState::PES_Failed)

	const USP_Target* const FleeTarget = Blackboard->GetObject<USP_Target>(FleeTargetEntryName);
	SP_RCHECK_NULLPTR(FleeTarget, ESP_PlanExecutionState::PES_Failed)

	const FVector PawnLocation = Pawn->GetActorLocation();
	const FVector FleeTargetLocation = FleeTarget->GetAnyPosition();

	const FVector FOVCenter = GetFOVCenter(Pawn);
	const FVector FOVMinExtent = GetFOVMinExtent(Pawn);
	const FVector FOVMaxExtent = GetFOVMaxExtent(Pawn);

	FVector FleeDirection = (Pawn->GetActorLocation() - FleeTargetLocation).GetSafeNormal();
	FVector FleePoint = ComputeFleePoint(FleeDirection, FOVCenter);

	if(bTargetVisible)
	{
		FHitResult HitInfos;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Pawn);

		if (const AActor* const FleeActor = FleeTarget->GetActor())
			Params.AddIgnoredActor(FleeActor);

		int RaycastNum = 0;

		// Find a point with no collision.
		for (; RaycastNum < MaxRaycastNum; ++RaycastNum)
		{
			if (Pawn->GetWorld()->LineTraceSingleByChannel(HitInfos, FOVCenter, FleePoint, ECollisionChannel::ECC_Visibility, Params))
			{
				// Compute new flee direction.
				float ImpactDirSize = (HitInfos.ImpactPoint - PawnLocation).Size();
				FleeDirection = (HitInfos.ImpactPoint + FleeDirection.MirrorByVector(HitInfos.ImpactNormal) * ImpactDirSize - PawnLocation).GetSafeNormal();

				FleePoint = ComputeFleePoint(FleeDirection, FOVCenter);
			}
			else // No collision: point found.
				break;
		}

		// Max raycast reached && Best point found not min range.
		if (RaycastNum >= MaxRaycastNum && IsInMinBox(Pawn, FleePoint))
			return ESP_PlanExecutionState::PES_Failed;
	}

	OutTarget->SetPosition(FleePoint);

#if SP_DEBUG_EDITOR

	DrawDebug(Planner, FleePoint);

#endif

	return ESP_PlanExecutionState::PES_Succeed;
}