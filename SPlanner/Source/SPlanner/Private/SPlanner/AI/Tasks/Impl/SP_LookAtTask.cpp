// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_LookAtTask.h>

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Blackboard/Objects/Target/SP_Target.h>

FRotator USP_LookAtTask::ComputeTargetRotation(const USP_AIPlannerComponent* Planner, const USP_Target* Target) const
{
	SP_RCHECK_NULLPTR(Planner, FRotator())

	APawn* Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, FRotator())
	SP_RCHECK_NULLPTR(Target, FRotator())

	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), Target->GetAnyPosition());

	if (bFreezePitch)
		Rotator.Pitch = 0.0f;

	if (bFreezeYaw)
		Rotator.Yaw = 0.0f;

	if (bFreezeRoll)
		Rotator.Roll = 0.0f;

	return Rotator;
}

USP_LookAtTask::USP_LookAtTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskInfosClass = USP_LookAtTaskInfos::StaticClass();
}

bool USP_LookAtTask::PreCondition_Implementation(const USP_PlannerComponent* Planner,
	const TArray<USP_ActionStep*>& GeneratedPlan,
	const USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlanGenInfos)

	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	// Target will be set.
	if(AIPlanGenInfos->IsBlackboardFlagSet(TargetEntryName, ESP_AIBBPlanGenFlags::PG_Dirty))
		return true;

	// Check valid Target.
	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	return Target->IsValid();
}
bool USP_LookAtTask::PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlanGenInfos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->AddFlag(ESP_AIPlanGenFlags::PG_PawnDirtyRotation);

	return true;
}
bool USP_LookAtTask::ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Planner, PlanGenInfos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(PlanGenInfos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->RemoveFlag(ESP_AIPlanGenFlags::PG_PawnDirtyRotation);

	return true;
}

bool USP_LookAtTask::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	USP_LookAtTaskInfos* const Infos = Cast<USP_LookAtTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_LookAtTaskInfos")

	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	APawn* Pawn = Planner->GetPawn();
	SP_SRCHECK_NULLPTR(Pawn, false)

	Infos->Start = Pawn->GetActorRotation();

	// Target become invalid during plan execution.
	if (!Target || !Target->IsValid())
		return false;

	// Precompute for static position or instant rotation.
	if (Target->IsPosition() || bInstant)
		Infos->End = ComputeTargetRotation(Planner, Target);

	return true;
}
ESP_PlanExecutionState USP_LookAtTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	APawn* Pawn = Planner->GetPawn();
	SP_SRCHECK_NULLPTR(Pawn, ESP_PlanExecutionState::PES_Failed)

	USP_LookAtTaskInfos* const Infos = Cast<USP_LookAtTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_LookAtTaskInfos")

	if(bInstant)
	{
		Pawn->SetActorRotation(Infos->End);
		return ESP_PlanExecutionState::PES_Succeed;
	}

	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	// Re-compute for moving objects.
	if (!Target->IsPosition())
	{
		FVector TargetPosition = Target->GetAnyPosition();

		Infos->End = ComputeTargetRotation(Planner, Target);
	}

	Infos->Alpha += DeltaSeconds * Speed;

	Pawn->SetActorRotation(UKismetMathLibrary::RLerp(Infos->Start, Infos->End, Infos->Alpha, true));

	return Infos->Alpha >= 1.0f ? ESP_PlanExecutionState::PES_Succeed : ESP_PlanExecutionState::PES_Running;
}