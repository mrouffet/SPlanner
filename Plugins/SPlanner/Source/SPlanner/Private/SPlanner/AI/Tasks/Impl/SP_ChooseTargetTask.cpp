// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
#endif

#include <SPlanner/AI/Target/SP_Target.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

FVector USP_ChooseTargetTask::GetFOVCenter(const APawn* Pawn) const
{
	SP_RCHECK_NULLPTR(Pawn, FVector::ZeroVector)

	return Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(LocalOffset);
}

bool USP_ChooseTargetTask::PreCondition_Implementation(const USP_PlannerComponent* Planner,
	const TArray<USP_ActionStep*>& GeneratedPlan,
	const USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlanGenInfos)

#if SP_DEBUG
	// Check valid blackboard entry.
	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)
#endif

	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);

	// Not already re-targeted.
	if(!bAllowReTarget && AIPlanGenInfos->IsBlackboardFlagSet(TargetEntryName, ESP_AIBBPlanGenFlags::PG_Dirty))
		return false;

	return true;
}
bool USP_ChooseTargetTask::PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlanGenInfos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->AddBlackboardFlags(TargetEntryName,
		ESP_AIBBPlanGenFlags::PG_Dirty,
		ESP_AIBBPlanGenFlags::PG_TargetPosition);

	return true;
}
bool USP_ChooseTargetTask::ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Planner, PlanGenInfos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->RemoveBlackboardFlags(TargetEntryName,
		ESP_AIBBPlanGenFlags::PG_Dirty,
		ESP_AIBBPlanGenFlags::PG_TargetPosition);

	return true;
}

#if SP_DEBUG_EDITOR
void USP_ChooseTargetTask::DrawDebug(const USP_AIPlannerComponent* Planner, const FVector& TargetLocation)
{
	APawn* const Pawn = Planner->GetPawn();
	SP_CHECK_NULLPTR(Pawn)

	FVector FOVCenter = GetFOVCenter(Pawn);

	SP_IF_TASK_EXECUTE(Planner)
	{
		if(MinDimensions.X > 0.0f && MinDimensions.Y > 0.0f && MinDimensions.Z > 0.0f)
			DrawDebugBox(Pawn->GetWorld(),
				FOVCenter,
				MinDimensions,
				MinFOVDebugColor, false,
				DebugDrawTime);

		if (MaxDimensions.X > 0.0f && MaxDimensions.Y > 0.0f && MaxDimensions.Z > 0.0f)
			DrawDebugBox(Pawn->GetWorld(),
				FOVCenter,
				MaxDimensions,
				MaxFOVDebugColor, false,
				DebugDrawTime);

		DrawDebugLine(Pawn->GetWorld(),
			FOVCenter,
			TargetLocation,
			LineDebugColor, false,
			DebugDrawTime);
	}
}
#endif