// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Misc/SP_AIDistToTargetFloatCurveParam.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>
#include <SPlanner/AI/Blackboard/Object/Target/SP_Target.h>

float USP_AIDistToTargetFloatCurveParam::Query_Implementation(const UObject* Outer) const
{
	const USP_AIPlannerComponent* const Planner = Cast<USP_AIPlannerComponent>(Outer);
	SP_RCHECK_NULLPTR(Planner, Super::Query_Implementation(Outer))

	const APawn* const Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, Super::Query_Implementation(Outer))

	const USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, Super::Query_Implementation(Outer))

	const USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK(Target && Target->IsValid(), Super::Query_Implementation(Outer), "Target invalid!")

	USP_FloatCurveParamInfos* Infos = NewObject<USP_FloatCurveParamInfos>();

	Infos->Outer = Planner;
	Infos->CurveTime = FVector::Dist(Target->GetAnyPosition(), Pawn->GetActorLocation());

	return Super::Query_Implementation(Infos);
}