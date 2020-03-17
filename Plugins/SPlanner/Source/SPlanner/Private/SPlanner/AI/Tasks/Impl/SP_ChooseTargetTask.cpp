// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
	#include <Kismet/KismetSystemLibrary.h>
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

FVector USP_ChooseTargetTask::GetFOVMinExtent(const APawn* Pawn) const
{
	SP_RCHECK_NULLPTR(Pawn, FVector::ZeroVector)

	return Pawn->GetActorRotation().RotateVector(MinLocalExtent);
}
FVector USP_ChooseTargetTask::GetFOVMaxExtent(const APawn* Pawn) const
{
	SP_RCHECK_NULLPTR(Pawn, FVector::ZeroVector)

	return Pawn->GetActorRotation().RotateVector(MaxLocalExtent);
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

	USP_Target* const OutTarget = Blackboard->GetObject<USP_Target>(OutTargetEntryName);
	SP_RCHECK_NULLPTR(OutTarget, false)
#endif

	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);

	// Not already re-targeted.
	if(!bAllowReTarget && AIPlanGenInfos->IsBlackboardFlagSet(OutTargetEntryName, ESP_AIBBPlanGenFlags::PG_Dirty))
		return false;

	return true;
}
bool USP_ChooseTargetTask::PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlanGenInfos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->AddBlackboardFlags(OutTargetEntryName,
		ESP_AIBBPlanGenFlags::PG_Dirty,
		ESP_AIBBPlanGenFlags::PG_TargetPosition);

	return true;
}
bool USP_ChooseTargetTask::ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Planner, PlanGenInfos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->RemoveBlackboardFlags(OutTargetEntryName,
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
		if(MinLocalExtent.X > 0.0f && MinLocalExtent.Y > 0.0f && MinLocalExtent.Z > 0.0f)
			UKismetSystemLibrary::DrawDebugBox(Pawn->GetWorld(),
				FOVCenter,
				MinLocalExtent,
				MinFOVDebugColor, Pawn->GetActorRotation(),
				DebugDrawTime);

		if (MaxLocalExtent.X > 0.0f && MaxLocalExtent.Y > 0.0f && MaxLocalExtent.Z > 0.0f)
			UKismetSystemLibrary::DrawDebugBox(Pawn->GetWorld(),
				FOVCenter,
				MaxLocalExtent,
				MaxFOVDebugColor, Pawn->GetActorRotation(),
				DebugDrawTime);

		DrawDebugLine(Pawn->GetWorld(),
			FOVCenter,
			TargetLocation,
			LineDebugColor, false,
			DebugDrawTime);
	}
}
#endif