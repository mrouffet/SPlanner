// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
	#include <Kismet/KismetSystemLibrary.h>
#endif

#include <SPlanner/AI/Blackboard/Objects/Target/SP_Target.h>

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

bool USP_ChooseTargetTask::IsInBox_Internal(const APawn* Pawn, const FVector& BoxLocalExtent, const FVector& InLocation) const
{
	const FVector FOVCenter = GetFOVCenter(Pawn);
	const FVector HalfLocalExtent = BoxLocalExtent / 2.0f;

	// Get input location in local pawn space.
	const FVector LocalInLocation = Pawn->GetActorRotation().UnrotateVector(InLocation);

	// Valid X Bound.
	if (BoxLocalExtent.X > 0.0f)
	{
		// Out of X bound.
		if (LocalInLocation.X < FOVCenter.X - HalfLocalExtent.X || LocalInLocation.X > FOVCenter.X + HalfLocalExtent.X)
			return false;
	}

	// Valid Y Bound.
	if (BoxLocalExtent.Y > 0.0f)
	{
		// Out of Y bound.
		if (LocalInLocation.Y < FOVCenter.Y - HalfLocalExtent.Y || LocalInLocation.Y > FOVCenter.Y + HalfLocalExtent.Y)
			return false;
	}

	// Valid Z Bound.
	if (BoxLocalExtent.Z > 0.0f)
	{
		// Out of Z bound.
		if (LocalInLocation.Z < FOVCenter.Z - HalfLocalExtent.Z || LocalInLocation.Z > FOVCenter.Z + HalfLocalExtent.Z)
			return false;
	}

	return true;
}
bool USP_ChooseTargetTask::IsInMinBox(const APawn* Pawn, const FVector& InLocation) const
{
	return IsInBox_Internal(Pawn, MinLocalExtent, InLocation);
}
bool USP_ChooseTargetTask::IsInMaxBox(const APawn* Pawn, const FVector& InLocation) const
{
	return IsInBox_Internal(Pawn, MaxLocalExtent, InLocation);
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
				MinLocalExtent / 2.0f,
				MinFOVDebugColor, Pawn->GetActorRotation(),
				DebugDrawTime);

		if (MaxLocalExtent.X > 0.0f && MaxLocalExtent.Y > 0.0f && MaxLocalExtent.Z > 0.0f)
			UKismetSystemLibrary::DrawDebugBox(Pawn->GetWorld(),
				FOVCenter,
				MaxLocalExtent / 2.0f,
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