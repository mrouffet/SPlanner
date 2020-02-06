#include <SPlanner/AI/Tasks/Impl/SP_LookAtTask.h>

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Target/SP_TargetComponent.h>

FRotator USP_LookAtTask::ComputeTargetRotation(const USP_AIPlannerComponent& Planner) const
{
	SP_SRCHECK_NULLPTR(Planner.Target, FRotator())
	SP_SRCHECK_NULLPTR(Planner.Target->GetOwner(), FRotator())

	FVector TargetPosition = Planner.Target->GetAnyPosition();
	FVector TargetOwnerLocation = Planner.Target->GetOwner()->GetActorLocation();

	if(bFreezePitch)
		TargetPosition.X = TargetOwnerLocation.X;

	if (bFreezeYaw)
		TargetPosition.Y = TargetOwnerLocation.Y;

	if (bFreezeRoll)
		TargetPosition.Z = TargetOwnerLocation.Z;

	return UKismetMathLibrary::FindLookAtRotation(TargetOwnerLocation, TargetPosition);
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

	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(&Planner);
	SP_SRCHECK_NULLPTR(AIPlanner, false)
	SP_SRCHECK_NULLPTR(AIPlanner->Target, false)

	return AIPlanner->Target->IsValid();
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

	AActor* TargetOwner = Planner.Target->GetOwner();
	SP_SRCHECK_NULLPTR(TargetOwner, false)

	FSP_TaskInfos* const Infos = new(UserData) FSP_TaskInfos();

	Infos->Start = TargetOwner->GetActorRotation();

	// Target become invalid during plan execution.
	if (!Planner.Target || !Planner.Target->IsValid())
		return false;

	// Precompute for static position or instant rotation.
	if (Planner.Target->IsPosition() || bInstant)
		Infos->End = ComputeTargetRotation(Planner);

	return true;
}
ESP_PlanExecutionState USP_LookAtTask::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_TICK_SUPER(DeltaSeconds, Planner, UserData)

	AActor* TargetOwner = Planner.Target->GetOwner();
	SP_SRCHECK_NULLPTR(TargetOwner, ESP_PlanExecutionState::PES_Failed)

	FSP_TaskInfos* const Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	if(bInstant)
	{
		TargetOwner->SetActorRotation(Infos->End);
		return ESP_PlanExecutionState::PES_Succeed;
	}

	// Re-compute for moving objects.
	if (!Planner.Target->IsPosition())
	{
		FVector TargetPosition = Planner.Target->GetAnyPosition();

		Infos->End = ComputeTargetRotation(Planner);
	}

	Infos->Alpha += DeltaSeconds * Speed;

	TargetOwner->SetActorRotation(UKismetMathLibrary::RLerp(Infos->Start, Infos->End, Infos->Alpha, true));

	return Infos->Alpha >= 1.0f ? ESP_PlanExecutionState::PES_Succeed : ESP_PlanExecutionState::PES_Running;
}
bool USP_LookAtTask::End(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_END_SUPER(Planner, UserData)

	reinterpret_cast<FSP_TaskInfos*>(UserData)->~FSP_TaskInfos();

	return true;
}