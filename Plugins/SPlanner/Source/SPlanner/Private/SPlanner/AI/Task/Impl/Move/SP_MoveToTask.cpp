// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/Move/SP_MoveToTask.h>

#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>

#include <Components/CapsuleComponent.h>

#include <SPlanner/AI/Controller/SP_AIController.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>
#include <SPlanner/AI/Blackboard/Object/Target/SP_Target.h>

USP_MoveToTask::USP_MoveToTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskInfosClass = USP_MoveToTaskInfos::StaticClass();
}

void USP_MoveToTask::SetPawnSpeed_Implementation(APawn* Pawn, float NewSpeed)
{
	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		UCharacterMovementComponent* const CharacterMovement = Character->GetCharacterMovement();
		SP_CHECK_NULLPTR(CharacterMovement)

		CharacterMovement->MaxWalkSpeed = NewSpeed;
	}
}

FAIMoveRequest USP_MoveToTask::CreateMoveRequest(const USP_Target* Target)
{
	FAIMoveRequest MoveRequest;

	SP_RCHECK_NULLPTR(Target, MoveRequest)

	 // Target position or non dynamic or frequency update.
	if (Target->GetState() == ESP_TargetState::TS_Position || !bIsDynamic || DynamicUpdateFrequency > 0.0f)
		MoveRequest.SetGoalLocation(Target->GetAnyPosition());
	else if (AActor* GoalActor = Target->GetAnyActor())
		MoveRequest.SetGoalActor(GoalActor);
	else
		SP_LOG(Error, "Bad Target!")

	MoveRequest.SetCanStrafe(bCanStrafe);
	MoveRequest.SetProjectGoalLocation(true);
	MoveRequest.SetUsePathfinding(bUsePathfinding);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	return MoveRequest;
}

bool USP_MoveToTask::HasReachedPosition(const USP_AIPlannerComponent* Planner, const USP_Target* Target) const
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(Target, false)

	APawn* Pawn = Planner->GetPawn();

	if(ACharacter* Character = Cast<ACharacter>(Pawn))
		return HasReachedPosition(Character, Target->GetAnyPosition());

	return HasReachedPosition(Planner->GetPawn(), Target->GetAnyPosition());
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

void USP_MoveToTask::OnMoveCompleted_Implementation(FAIRequestID RequestID, EPathFollowingResult::Type ExecResult)
{
	USP_MoveToTaskInfos* const Infos = RequestIDToTaskInfos[RequestID];
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

bool USP_MoveToTask::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	// New target will be set.
	if (AIPlanGenInfos->IsBlackboardFlagSet(TargetEntryName, ESP_AIBBPlanGenFlags::PG_Dirty))
		return true;

	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Infos->Planner);

	USP_AIBlackboardComponent* const Blackboard = AIPlanner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	// Get Target.
	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	// Valid target checked in SP_MoveBaseTask.

	return bPreconditionFailWhileAlreadyAtGoal ? !HasReachedPosition(AIPlanner, Target) : true;
}

bool USP_MoveToTask::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	USP_MoveToTaskInfos* const Infos = Cast<USP_MoveToTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_MoveToTaskInfos")

	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	// Use AIController pathfinding MoveTo.
	ASP_AIController* const Controller = Planner->GetController();
	SP_RCHECK_NULLPTR(Controller, false)


	// Create MoveRequest.
	Infos->MoveRequest = CreateMoveRequest(Target);

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
	Infos->MoveId = Request.MoveId;

	Infos->bIsDynamic = bIsDynamic && DynamicUpdateFrequency > 0.0f;

	if (PawnSpeed > 0.0f)
	{
		APawn* const AIPawn = Planner->GetPawn();
		SP_RCHECK(AIPawn, false, "AIPawn nullptr! Planner must be attached to a pawn!")

		// Store previous value.
		Infos->PrevPawnSpeed = GetPawnSpeed(AIPawn);

		SetPawnSpeed(AIPawn, PawnSpeed);
	}

	return true;
}
ESP_PlanExecutionState USP_MoveToTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	USP_MoveToTaskInfos* const Infos = Cast<USP_MoveToTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_MoveToTaskInfos")

	if (Infos->ExecutionState == ESP_PlanExecutionState::PES_Failed)
	{
		SP_LOG_TASK_TICK(Planner, "Failed")
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

		SP_LOG_TASK_TICK(Planner, "Succeed")

		return ESP_PlanExecutionState::PES_Succeed;
	}

	// TODO: Clean later.
	if (Infos->bIsDynamic)
	{
		Infos->DynamicTime += DeltaSeconds;

		if (DynamicUpdateFrequency <= 0.0f || Infos->DynamicTime >= DynamicUpdateFrequency)
		{
			USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
			SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

			USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
			SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

			Infos->MoveRequest.SetGoalLocation(Target->GetAnyPosition());

			ASP_AIController* const Controller = Planner->GetController();
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

			if (Request.MoveId != Infos->MoveId)
			{
				RequestIDToTaskInfos.Remove(Infos->MoveId);

				Infos->MoveId = Request.MoveId;
				RequestIDToTaskInfos.Add(Request.MoveId, Infos);
			}
		}
	}
	//

	return ESP_PlanExecutionState::PES_Running;
}
bool USP_MoveToTask::End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_END(Planner, TaskInfos)

	USP_MoveToTaskInfos* const Infos = Cast<USP_MoveToTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_MoveToTaskInfos")

	if (PawnSpeed >= 0.0f && Infos->PrevPawnSpeed >= 0.0f)
	{
		APawn* const AIPawn = Planner->GetPawn();
		SP_RCHECK(AIPawn, false, "AIPawn nullptr! Planner must be attached to a pawn!")

		// Reset to saved speed.
		SetPawnSpeed(AIPawn, Infos->PrevPawnSpeed);
	}

	if (Infos->ExecutionState == ESP_PlanExecutionState::PES_Running && Infos->Controller)
	{
		if(bStopMovementOnCancel || Infos->bForcedEnd) // Always stop when task get forced cancelled (Controller Freeze).
			Infos->Controller->StopMovement(); // Call ReceiveMoveCompleted.
		else
		{
			// Unbind completed event manually.
			RequestIDToTaskInfos.Remove(Infos->MoveId);

			SP_RCHECK_NULLPTR(Infos->Controller, false)
			Infos->Controller->ReceiveMoveCompleted.RemoveDynamic(this, &USP_MoveToTask::OnMoveCompleted);
		}
	}

	return true;
}