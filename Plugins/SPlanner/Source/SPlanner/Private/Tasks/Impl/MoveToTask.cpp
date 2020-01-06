#include <Tasks/Impl/MoveToTask.h>

#include <AIController.h>
#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>

#include <Miscs/PlannerFlags.h>

#include <Components/PlannerComponent.h>
#include <Components/TargetComponent.h>
#include <Components/POIComponent.h>

bool USP_MoveToTask::HasReachedPosition(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	if(AAIController* Controller = Cast<AAIController>(Planner->GetOwner()))
	{
		if (ACharacter* Character = Cast<ACharacter>(Controller->GetPawn()))
			return HasReachedPosition(Character, Planner->GetTarget()->GetAnyPosition());
	}

	return HasReachedPosition(Planner->GetOwner(), Planner->GetTarget()->GetAnyPosition());
}
bool USP_MoveToTask::HasReachedPosition(AActor* Actor, const FVector& TargetPosition) const
{
	SP_RCHECK_NULLPTR(Actor, false)

	// Move distance (XY only).
	float SqrMoveDist = (Actor->GetActorLocation() - TargetPosition).SizeSquared2D();

	return SqrMoveDist <= AcceptanceRadius * AcceptanceRadius;
}
bool USP_MoveToTask::HasReachedPosition(ACharacter* Character, const FVector& TargetPosition) const
{
	SP_RCHECK_NULLPTR(Character, false)

	// Move distance (XY only).
	float SqrMoveDist = (Character->GetActorLocation() - TargetPosition).SizeSquared2D();

	// Acceptance + Character collider * damping.
	float MinRadius = FMath::Square(AcceptanceRadius + Character->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.2f);

	// Check not already at target (XY only).
	return SqrMoveDist <= MinRadius;
}

uint32 USP_MoveToTask::GetUserDataSize() const
{
	return sizeof(FSP_TaskInfos);
}

void USP_MoveToTask::OnMoveCompleted_Implementation(FAIRequestID RequestID, EPathFollowingResult::Type ExecResult)
{
	FSP_TaskInfos* const Infos = RequestIDToTaskInfos[RequestID];
	SP_CHECK_NULLPTR(Infos)

	switch (ExecResult)
	{
	case EPathFollowingResult::Type::Success:
		Infos->ExecutionState = ESP_PlanExecutionState::PES_Succeed;
		break;
	default:
		Infos->ExecutionState = ESP_PlanExecutionState::PES_Failed;
		break;
	}

	RequestIDToTaskInfos.Remove(RequestID);

	// Unbind completed event.
	SP_CHECK_NULLPTR(Infos->Controller)
	Infos->Controller->ReceiveMoveCompleted.RemoveDynamic(this, &USP_MoveToTask::OnMoveCompleted);
}

bool USP_MoveToTask::PreCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const
{
	SP_TASK_SUPER_PRECONDITION(Planner, PlannerFlags)

	SP_RCHECK_NULLPTR(Planner->GetTarget(), false)

	// New target will be set.
	if(SP_IS_FLAG_SET(PlannerFlags, ESP_PlannerFlags::PF_DirtyTarget))
		return true;

	// Check valid target and has not already moved.
	if (!Planner->GetTarget()->IsValid() || SP_IS_FLAG_SET(PlannerFlags, ESP_PlannerFlags::PF_DirtyTransform))
		return false;

	return !HasReachedPosition(Planner);
}
int USP_MoveToTask::PostCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const
{
	SP_TASK_SUPER_POSTCONDITION(Planner, PlannerFlags)
	
	return SP_ADD_FLAG(PlannerFlags, ESP_PlannerFlags::PF_DirtyTransform);
}

ESP_PlanExecutionState USP_MoveToTask::Begin(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Begin, Planner, UserData)

	FSP_TaskInfos* const Infos = new(UserData) FSP_TaskInfos{};

	if (HasReachedPosition(Planner))
	{
		Infos->ExecutionState = ESP_PlanExecutionState::PES_Succeed;
		return ESP_PlanExecutionState::PES_Succeed;
	}


	// Use AIController pathfinding MoveTo.
	AAIController* const Controller = Cast<AAIController>(Planner->GetOwner());
	SP_RCHECK_NULLPTR(Controller, ESP_PlanExecutionState::PES_Failed)


	// Create MoveRequest.
	FAIMoveRequest MoveRequest;

	if(Planner->GetTarget()->GetState() == ESP_TargetState::TS_Position)
		MoveRequest.SetGoalLocation(Planner->GetTarget()->GetPosition());
	else if(AActor* GoalActor = Planner->GetTarget()->GetAnyActor())
		MoveRequest.SetGoalActor(GoalActor);
	else
	{
		SP_LOG(Error, "Bad Target!")
		return ESP_PlanExecutionState::PES_Failed;
	}

	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);


	// Request movement.
#if SP_DEBUG
	FPathFollowingRequestResult Request = Controller->MoveTo(MoveRequest, &Infos->DebugPath);
#else
	FPathFollowingRequestResult Request = Controller->MoveTo(MoveRequest);
#endif


	if (Request.Code == EPathFollowingRequestResult::Failed)
	{
		SP_LOG_TASK_TICK(Controller, "Move request failed!")
		return ESP_PlanExecutionState::PES_Failed;
	}
	else if (Request.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		Infos->ExecutionState = ESP_PlanExecutionState::PES_Succeed;
		return ESP_PlanExecutionState::PES_Succeed;
	}

	SP_LOG_TASK_EXECUTE(Controller, "Pathfinding: %s", *Planner->GetTarget()->GetAnyPosition().ToString())

	// Bind completed event.
	Infos->Controller = Controller;
	Controller->ReceiveMoveCompleted.AddDynamic(this, &USP_MoveToTask::OnMoveCompleted);


	// Save RequestID and task infos.
	RequestIDToTaskInfos.Add(Request.MoveId, Infos);

	return ESP_PlanExecutionState::PES_Succeed;
}
ESP_PlanExecutionState USP_MoveToTask::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	FSP_TaskInfos* const Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	if (Infos->ExecutionState == ESP_PlanExecutionState::PES_Failed)
	{
		SP_LOG_TASK_TICK(Planner->GetOwner(), "Failed")
		return ESP_PlanExecutionState::PES_Failed;
	}
	else if (Infos->ExecutionState == ESP_PlanExecutionState::PES_Succeed)
	{
		const AAIController* const Controller = Cast<AAIController>(Planner->GetOwner());
		SP_RCHECK_NULLPTR(Controller, ESP_PlanExecutionState::PES_Failed)

		const ACharacter* const Character = Cast<ACharacter>(Controller->GetPawn());
		SP_RCHECK_NULLPTR(Character, ESP_PlanExecutionState::PES_Failed)

		// Wait for complete stop.
		if (Character->GetVelocity().SizeSquared() != 0.0f)
			return ESP_PlanExecutionState::PES_Running;

		SP_LOG_TASK_TICK(Character, "Succeed")

		return ESP_PlanExecutionState::PES_Succeed;
	}

	return ESP_PlanExecutionState::PES_Running;
}
ESP_PlanExecutionState USP_MoveToTask::End(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(End, Planner, UserData)

	reinterpret_cast<FSP_TaskInfos*>(UserData)->~FSP_TaskInfos();

	return ESP_PlanExecutionState::PES_Succeed;
}