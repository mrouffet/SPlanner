// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Decorators/Impl/SP_DistanceAIDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>
#include <SPlanner/AI/Target/SP_Target.h>

bool USP_DistanceAIDecorator::Validate_Internal_Implementation(const USP_PlannerComponent* Planner)
{
	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);
	SP_RCHECK(AIPlanner, false, "AIPlanner nullptr! Planner must be of type USP_AIPlannerComponent!")

	const APawn* const Pawn = AIPlanner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, false)

	const USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	const USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	float SqrDist = FVector::DistSquared(Pawn->GetActorLocation(), Target->GetAnyPosition());

	// Valid Min Distance.
	if (MinDistance > 0.0f)
	{
		if (SqrDist < MinDistance * MinDistance)
			return false;
	}

	// Valid Min Distance.
	if (MaxDistance > 0.0f)
	{
		if (SqrDist > MaxDistance * MaxDistance)
			return false;
	}

	return true;
}

bool USP_DistanceAIDecorator::PreCondition_Validate_Internal_Implementation(const USP_PlannerComponent* Planner, const USP_PlanGenInfos* PlanGenInfos)
{
	// Check before super (Validate_Internal).

	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);
	SP_RCHECK(AIPlanGenInfos, false, "AIPlanGenInfos nullptr! PlanGenInfos must be of type USP_AIPlanGenInfos!")

	// Target is dirty and can't be checked: predict true.
	if (AIPlanGenInfos->IsBlackboardFlagSet(TargetEntryName, ESP_AIBBPlanGenFlags::PG_Dirty))
		return true;

	return Super::PreCondition_Validate_Internal_Implementation(Planner, PlanGenInfos);
}