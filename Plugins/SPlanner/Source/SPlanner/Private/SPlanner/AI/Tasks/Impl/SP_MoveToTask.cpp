// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_MoveToTask.h>

#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>

#include <SPlanner/AI/Controllers/SP_AIController.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>

bool USP_MoveToTask::HasReachedPosition(const USP_AIPlannerComponent* Planner, const USP_Target* Target, const FVector& Offset) const
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(Target, false)

	APawn* Pawn = Planner->GetPawn();

	if(ACharacter* Character = Cast<ACharacter>(Pawn))
		return HasReachedPosition(Character, Target->GetAnyPosition() + Offset);

	return HasReachedPosition(Planner->GetPawn(), Target->GetAnyPosition() + Offset);
}
bool USP_MoveToTask::HasReachedPosition(APawn* Pawn, const FVector& TargetPosition) const
{
	SP_RCHECK_NULLPTR(Pawn, false)

	// Move distance (XY only).
	float SqrMoveDist = (Pawn->GetActorLocation() - TargetPosition).SizeSquared2D();

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

FAIMoveRequest USP_MoveToTask::CreateMoveRequest(const USP_Target* Target, const FVector& Offset)
{
	FAIMoveRequest MoveRequest;

	SP_RCHECK_NULLPTR(Target, MoveRequest)

	if (Target->GetState() == ESP_TargetState::TS_Position || !FMath::IsNearlyZero(Offset.SizeSquared())) // Target position or offset.
		MoveRequest.SetGoalLocation(Target->GetAnyPosition() + Offset);
	else if (AActor* GoalActor = Target->GetAnyActor())
		MoveRequest.SetGoalActor(GoalActor);
	else
		SP_LOG(Error, "Bad Target!")

	MoveRequest.SetCanStrafe(bCanStrafe);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	return MoveRequest;
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

bool USP_MoveToTask::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	// Not already moved to.
	if(!bAllowMultipleMoveTo && SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_LocationDirty))
		return false;

	// New target will be set.
	if (SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty))
		return true;

	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(&Planner);

	USP_AIBlackboardComponent* const Blackboard = AIPlanner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	// Get Target.
	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	// Check valid target and has not already moved.
	if (!Target->IsValid())
		return false;

	// Get Offset.
	FVector Offset = FVector::ZeroVector;
	if (OffsetEntryName != "None")
		Offset = Blackboard->GetVector(OffsetEntryName);

	return bPreconditionFailWhileAlreadyAtGoal ? !HasReachedPosition(AIPlanner, Target, Offset) : true;
}
uint64 USP_MoveToTask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)
	
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_LocationDirty);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_RotationDirty);

	return PlannerFlags;
}

bool USP_MoveToTask::Begin(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_BEGIN_SUPER(Planner, UserData)

	FSP_TaskInfos* const Infos = new(UserData) FSP_TaskInfos{};

	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	FVector Offset = FVector::ZeroVector;
	if(OffsetEntryName != "None")
		Offset = Blackboard->GetVector(OffsetEntryName);

	if (HasReachedPosition(&Planner, Target, Offset))
	{
		Infos->ExecutionState = ESP_PlanExecutionState::PES_Succeed;
		return true;
	}


	// Use AIController pathfinding MoveTo.
	ASP_AIController* const Controller = Planner.GetController();
	SP_RCHECK_NULLPTR(Controller, false)


	// Create MoveRequest.
	Infos->MoveRequest = CreateMoveRequest(Target, Offset);

	// Request movement.
#if SP_DEBUG
	FPathFollowingRequestResult Request = Controller->MoveTo(Infos->MoveRequest, &Infos->DebugPath);
#else
	FPathFollowingRequestResult Request = Controller->MoveTo(Infos->MoveRequest);
#endif

	if (Request.Code == EPathFollowingRequestResult::Failed)
	{
		SP_LOG_TASK_EXECUTE(Planner, "Move request failed!")
		return false;
	}
	else if (Request.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		Infos->ExecutionState = ESP_PlanExecutionState::PES_Succeed;
		return true;
	}

	SP_LOG_TASK_EXECUTE(Planner, "Pathfinding: %s", *Target->GetAnyPosition().ToString())

	// Bind completed event.
	Infos->Controller = Controller;
	Controller->ReceiveMoveCompleted.AddDynamic(this, &USP_MoveToTask::OnMoveCompleted);

	// Save RequestID and task infos.
	RequestIDToTaskInfos.Add(Request.MoveId, Infos);

	// Set dynamic.
	Infos->bIsDynamic = Target->IsActor() && !FMath::IsNearlyZero(Offset.SizeSquared());

	return true;
}
ESP_PlanExecutionState USP_MoveToTask::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_TICK_SUPER(DeltaSeconds, Planner, UserData)

	FSP_TaskInfos* const Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	if (Infos->ExecutionState == ESP_PlanExecutionState::PES_Failed)
	{
		SP_LOG_TASK_TICK(Planner, "Failed")
		return ESP_PlanExecutionState::PES_Failed;
	}
	else if (Infos->ExecutionState == ESP_PlanExecutionState::PES_Succeed)
	{
		const AAIController* const Controller = Cast<AAIController>(Planner.GetOwner());
		SP_RCHECK_NULLPTR(Controller, ESP_PlanExecutionState::PES_Failed)

		const ACharacter* const Character = Cast<ACharacter>(Controller->GetPawn());
		SP_RCHECK_NULLPTR(Character, ESP_PlanExecutionState::PES_Failed)

		// Wait for complete stop.
		if (Character->GetVelocity().SizeSquared() != 0.0f)
			return ESP_PlanExecutionState::PES_Running;

		SP_LOG_TASK_TICK(Planner, "Succeed")

		return ESP_PlanExecutionState::PES_Succeed;
	}

	// TODO: Clean later.
	if (Infos->bIsDynamic)
	{
		USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
		SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

		USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
		SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

		Infos->MoveRequest.SetGoalLocation(Target->GetAnyPosition() + Blackboard->GetVector(OffsetEntryName));

		ASP_AIController* const Controller = Planner.GetController();
		SP_RCHECK_NULLPTR(Controller, ESP_PlanExecutionState::PES_Failed)

		// Request movement.
		#if SP_DEBUG
			FPathFollowingRequestResult Request = Controller->MoveTo(Infos->MoveRequest, &Infos->DebugPath);
		#else
			FPathFollowingRequestResult Request = Controller->MoveTo(Infos->MoveRequest);
		#endif

		if (Request.Code == EPathFollowingRequestResult::Failed)
		{
			SP_LOG_TASK_EXECUTE(Planner, "Move request failed!")
			return ESP_PlanExecutionState::PES_Failed;
		}
		else if (Request.Code == EPathFollowingRequestResult::AlreadyAtGoal)
			return ESP_PlanExecutionState::PES_Succeed;
	}
	//

	return ESP_PlanExecutionState::PES_Running;
}
bool USP_MoveToTask::End(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_END_SUPER(Planner, UserData)

	reinterpret_cast<FSP_TaskInfos*>(UserData)->~FSP_TaskInfos();

	return true;
}

bool USP_MoveToTask::Cancel(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_CANCEL_SUPER(Planner, UserData)

	FSP_TaskInfos* const Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	if (Infos->ExecutionState == ESP_PlanExecutionState::PES_Running)
		Infos->Controller->StopMovement(); // Call ReceiveMoveCompleted.
	
	Infos->~FSP_TaskInfos();

	return true;
}