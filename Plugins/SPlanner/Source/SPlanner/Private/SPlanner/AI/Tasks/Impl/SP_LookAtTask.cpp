// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_LookAtTask.h>

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

FRotator USP_LookAtTask::ComputeTargetRotation(const USP_AIPlannerComponent& Planner, const USP_Target* Target) const
{
	APawn* Pawn = Planner.GetPawn();
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

uint32 USP_LookAtTask::GetUserDataSize() const
{
	return sizeof(FSP_TaskInfos);
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

bool USP_LookAtTask::Begin(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_BEGIN_SUPER(Planner, UserData)

	if(bInstant)
		return true;

	// Construct before any return for correct destruction in End().
	FSP_TaskInfos* const Infos = new(UserData) FSP_TaskInfos();

	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	APawn* Pawn = Planner.GetPawn();
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
ESP_PlanExecutionState USP_LookAtTask::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_TICK_SUPER(DeltaSeconds, Planner, UserData)

	APawn* Pawn = Planner.GetPawn();
	SP_SRCHECK_NULLPTR(Pawn, ESP_PlanExecutionState::PES_Failed)

	FSP_TaskInfos* const Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	if(bInstant)
	{
		Pawn->SetActorRotation(Infos->End);
		return ESP_PlanExecutionState::PES_Succeed;
	}

	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
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
bool USP_LookAtTask::End(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_END_SUPER(Planner, UserData)

	if(!bInstant)
		reinterpret_cast<FSP_TaskInfos*>(UserData)->~FSP_TaskInfos();

	return true;
}