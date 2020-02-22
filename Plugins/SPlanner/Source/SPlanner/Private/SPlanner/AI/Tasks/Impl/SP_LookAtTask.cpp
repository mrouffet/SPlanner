// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_LookAtTask.h>

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

FRotator USP_LookAtTask::ComputeTargetRotation(const USP_AIPlannerComponent* Planner, const USP_Target* Target) const
{
	SP_SRCHECK_NULLPTR(Planner, FRotator())

	APawn* Pawn = Planner->GetPawn();
	SP_SRCHECK_NULLPTR(Pawn, FRotator())
	SP_SRCHECK_NULLPTR(Target, FRotator())

	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), Target->GetAnyPosition());

	if (bFreezePitch)
		Rotator.Pitch = 0.0f;

	if (bFreezeYaw)
		Rotator.Yaw = 0.0f;

	if (bFreezeRoll)
		Rotator.Roll = 0.0f;

	return Rotator;
}

USP_TaskInfosBase* USP_LookAtTask::InstantiateInfos()
{
	return NewObject<USP_LookAtTaskInfos>();
}

bool USP_LookAtTask::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	if(SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty))
		return true;

	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	return Target->IsValid();
}
uint64 USP_LookAtTask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_RotationDirty);

	return PlannerFlags;
}

bool USP_LookAtTask::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfosBase* TaskInfos)
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
ESP_PlanExecutionState USP_LookAtTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfosBase* TaskInfos)
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