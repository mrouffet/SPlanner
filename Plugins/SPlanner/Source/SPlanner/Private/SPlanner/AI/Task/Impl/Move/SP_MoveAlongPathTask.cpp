// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/Move/SP_MoveAlongPathTask.h>

#include <GameFramework/CharacterMovementComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>
#include <SPlanner/AI/Blackboard/Object/Target/SP_Target.h>

#include <SPlanner/AI/POI/Impl/SP_AIPath.h>
#include <SPlanner/Misc/SP_SplineComponent.h>

USP_MoveAlongPathTask::USP_MoveAlongPathTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskInfosClass = USP_MoveAlongPathTaskInfos::StaticClass();
}

void USP_MoveAlongPathTask::ApplyLocation_Implementation(APawn* Pawn, const FVector& NewLocation)
{
	SP_CHECK_NULLPTR(Pawn)

	if(bUseMovementComponent)
		Pawn->AddMovementInput(NewLocation - Pawn->GetActorLocation());
	else
		Pawn->SetActorLocation(NewLocation);
}
void USP_MoveAlongPathTask::ApplyRotation_Implementation(APawn* Pawn, const FRotator& NewRotation)
{
	SP_CHECK_NULLPTR(Pawn)
	
	if (bUseMovementComponent)
	{
		FRotator DeltaRotation = NewRotation - Pawn->GetActorRotation();

		Pawn->AddControllerPitchInput(DeltaRotation.Pitch);
		Pawn->AddControllerYawInput(DeltaRotation.Yaw);
		Pawn->AddControllerRollInput(DeltaRotation.Roll);
	}
	else
		Pawn->SetActorRotation(NewRotation);
}

bool USP_MoveAlongPathTask::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	// Dirty target will be a POI.
	if (AIPlanGenInfos->IsBlackboardFlagSet(TargetEntryName, ESP_AIBBPlanGenFlags::PG_TargetPOI))
		return true;

	USP_AIBlackboardComponent* const Blackboard = Infos->Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	// No traget change, check current is not a POI.
	if (Target->GetState() != ESP_TargetState::TS_POI)
		return false;

	// POI must be of type SP_AIPath.
	return Cast<ASP_AIPath>(Target->GetAnyActor());
}

bool USP_MoveAlongPathTask::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	USP_MoveAlongPathTaskInfos* const Infos = Cast<USP_MoveAlongPathTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_MoveAlongPathTaskInfos")

	APawn* const AIPawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(AIPawn, false)

	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	ASP_AIPath* const Path = Cast<ASP_AIPath>(Target->GetAnyActor());
	SP_RCHECK_NULLPTR(Path, false)

	USP_SplineComponent* const Spline = Path->GetSpline();
	SP_RCHECK_NULLPTR(Spline, false)

	Infos->InputKey = Infos->StartInputKey = Spline->FindInputKeyClosestToWorldLocation(AIPawn->GetActorLocation());
	Infos->DirectionSign = DirectionSign == 0.0f ? (FMath::RandBool() ? 1.0f : -1.0f) : DirectionSign;

	return true;
}
ESP_PlanExecutionState USP_MoveAlongPathTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	USP_MoveAlongPathTaskInfos* const Infos = Cast<USP_MoveAlongPathTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_MoveAlongPathTaskInfos")

	APawn* const AIPawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(AIPawn, ESP_PlanExecutionState::PES_Failed)

	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	ASP_AIPath* const Path = Cast<ASP_AIPath>(Target->GetAnyActor());
	SP_RCHECK_NULLPTR(Path, ESP_PlanExecutionState::PES_Failed)

	USP_SplineComponent* const Spline = Path->GetSpline();
	SP_RCHECK_NULLPTR(Spline, ESP_PlanExecutionState::PES_Failed)

	float Speed = PawnSpeed <= 0.0f ? GetPawnSpeed(AIPawn) : PawnSpeed;
	Infos->InputKey += DeltaSeconds * Speed / Spline->GetSplineLength() * Infos->DirectionSign;

	if (Spline->IsClosedLoop())
	{
		// Modulo spline length.
		if (Infos->InputKey <= 0.0f)
		{
			Infos->bHasLooped = true;
			Infos->InputKey += Spline->GetNumberOfSplinePoints();
		}
		else if (Infos->InputKey >= Spline->GetNumberOfSplinePoints())
		{
			Infos->bHasLooped = true;
			Infos->InputKey -= Spline->GetNumberOfSplinePoints();
		}
	}
	else if (Infos->InputKey <= 0.0f || Infos->InputKey >= Spline->GetSplineLength()) // End of spline.
		return ESP_PlanExecutionState::PES_Succeed;

	// No else (do not depend on Spline->IsClosedLoop()).
	if (!bLoopSpline && Infos->bHasLooped &&
		!((Infos->DirectionSign > 0.0f) ^ (Infos->InputKey > Infos->StartInputKey))) // Both direction and inputkey comparison must have the same sign.
			return ESP_PlanExecutionState::PES_Succeed;

	const FVector NewLocation = Spline->GetLocationAtSplineInputKey(Infos->InputKey, ESplineCoordinateSpace::World);
	FRotator NewRotation = Spline->GetRotationAtSplineInputKey(Infos->InputKey, ESplineCoordinateSpace::World);

	if (Infos->DirectionSign < 0.0f)
		NewRotation = NewRotation.GetInverse();

	ApplyLocation(AIPawn, NewLocation);
	ApplyRotation(AIPawn, NewRotation);

	return ESP_PlanExecutionState::PES_Running;
}