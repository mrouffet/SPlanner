// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_MoveToTask.h>

#include <Components/CapsuleComponent.h>

#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>

#include <SPlanner/AI/Controllers/SP_AIController.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>

bool USP_MoveToTask::HasReachedPosition(const USP_AIPlannerComponent& Planner, const USP_Target* Target) const
{
	SP_RCHECK_NULLPTR(Target, false)

	APawn* Pawn = Planner.GetPawn();

	if(ACharacter* Character = Cast<ACharacter>(Pawn))
		return HasReachedPosition(Character, Target->GetAnyPosition());

	return HasReachedPosition(Planner.GetPawn(), Target->GetAnyPosition());
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

bool USP_MoveToTask::IsTargetVisible(const USP_AIPlannerComponent& Planner, const USP_Target* Target) const
{
	SP_RCHECK_NULLPTR(Target, false)

	APawn* const AIPawn = Planner.GetPawn();
	SP_RCHECK(AIPawn, false, "AIPawn nullptr! Planner must be attached to a pawn!")

		// Avoid Self or target collisions.
	FHitResult HitInfos;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AIPawn);

	if (AActor* TargetActor = Target->GetAnyActor())
		Params.AddIgnoredActor(TargetActor);

	if (Planner.GetWorld()->LineTraceSingleByChannel(HitInfos,
		AIPawn->GetActorLocation(),
		Target->GetAnyPosition(),
		ECollisionChannel::ECC_Visibility, Params))
		return false;

	return true;
}

FAIMoveRequest USP_MoveToTask::CreateMoveRequest(const USP_Target* Target)
{
	FAIMoveRequest MoveRequest;

	SP_RCHECK_NULLPTR(Target, MoveRequest)

	if (Target->GetState() == ESP_TargetState::TS_Position || !bIsDynamic) // Target position or non dynamic.
		MoveRequest.SetGoalLocation(Target->GetAnyPosition());
	else if (AActor* GoalActor = Target->GetAnyActor())
		MoveRequest.SetGoalActor(GoalActor);
	else
		SP_LOG(Error, "Bad Target!")

	MoveRequest.SetCanStrafe(bCanStrafe);
	MoveRequest.SetUsePathfinding(bUsePathfinding);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	return MoveRequest;
}

float USP_MoveToTask::GetPawnSpeed_Implementation(APawn* Pawn)
{
	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		UCharacterMovementComponent* const CharacterMovement = Character->GetCharacterMovement();
		SP_RCHECK_NULLPTR(CharacterMovement, false)

		return CharacterMovement->MaxWalkSpeed;
	}

	return -1.0f;
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

uint32 USP_MoveToTask::GetUserDataSize() const
{
	return sizeof(FSP_MoveToTaskInfos);
}

void USP_MoveToTask::ConstructUserData(uint8* UserData)
{
	new(UserData) FSP_MoveToTaskInfos{};
}
void USP_MoveToTask::DestructUserData(uint8* UserData)
{
	reinterpret_cast<FSP_MoveToTaskInfos*>(UserData)->~FSP_MoveToTaskInfos();
}

void USP_MoveToTask::OnMoveCompleted_Implementation(FAIRequestID RequestID, EPathFollowingResult::Type ExecResult)
{
	FSP_MoveToTaskInfos* const Infos = RequestIDToTaskInfos[RequestID];
	SP_CHECK_NULLPTR(Infos)

	switch (ExecResult)
	{
	case EPathFollowingResult::Type::Success:
		Infos->MT_ExecutionState = ESP_PlanExecutionState::PES_Succeed;
		break;
	default:
		Infos->MT_ExecutionState = ESP_PlanExecutionState::PES_Failed;
		break;
	}

	RequestIDToTaskInfos.Remove(RequestID);

	// Unbind completed event.
	SP_CHECK_NULLPTR(Infos->MT_Controller)
	Infos->MT_Controller->ReceiveMoveCompleted.RemoveDynamic(this, &USP_MoveToTask::OnMoveCompleted);
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

	FVector TargetLocation = Target->GetAnyPosition();

	if (bTargetVisible && !IsTargetVisible(*AIPlanner, Target))
		return false;

	// Check is in range.
	if (MinDistance > 0.0f || MaxDistance > 0.0f)
	{
		APawn* AIPawn = AIPlanner->GetPawn();
		SP_RCHECK(AIPawn, false, "AIPawn nullptr! Planner must be attached to a pawn!")

		float TargetDistSqr = FVector::DistSquared(AIPawn->GetActorLocation(), TargetLocation);

		// Min Distance range.
		if (MinDistance > 0.0f && TargetDistSqr < MinDistance * MinDistance)
			return !bPreconditionFailWhileAlreadyAtGoal;

		// Max Distance range.
		if (MaxDistance > 0.0f && TargetDistSqr > MaxDistance * MaxDistance)
			return false;
	}

	return bPreconditionFailWhileAlreadyAtGoal ? !HasReachedPosition(*AIPlanner, Target) : true;
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
	SP_TASK_SUPER_BEGIN(Planner, UserData)

	FSP_MoveToTaskInfos* const Infos = reinterpret_cast<FSP_MoveToTaskInfos*>(UserData);

	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	if (HasReachedPosition(Planner, Target))
	{
		Infos->MT_ExecutionState = ESP_PlanExecutionState::PES_Succeed;
		return true;
	}

	if (bTargetVisible && !IsTargetVisible(Planner, Target))
	{
		SP_LOG_TASK_EXECUTE(Planner, "Move request failed: Taget not visible.")
		Infos->MT_ExecutionState = ESP_PlanExecutionState::PES_Failed;
		return false;
	}

	// Use AIController pathfinding MoveTo.
	ASP_AIController* const Controller = Planner.GetController();
	SP_RCHECK_NULLPTR(Controller, false)


	// Create MoveRequest.
	Infos->MT_MoveRequest = CreateMoveRequest(Target);

	// Request movement.
#if SP_DEBUG
	FPathFollowingRequestResult Request = Controller->MoveTo(Infos->MT_MoveRequest, &Infos->MT_DebugPath);
#else
	FPathFollowingRequestResult Request = Controller->MoveTo(Infos->MT_MoveRequest);
#endif

	if (Request.Code == EPathFollowingRequestResult::Failed)
	{
		SP_LOG_TASK_EXECUTE(Planner, "Move request failed!")
		return false;
	}
	else if (Request.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		Infos->MT_ExecutionState = ESP_PlanExecutionState::PES_Succeed;
		return true;
	}

	SP_LOG_TASK_EXECUTE(Planner, "Pathfinding: %s", *Target->GetAnyPosition().ToString())

	// Bind completed event.
	Infos->MT_Controller = Controller;
	Controller->ReceiveMoveCompleted.AddDynamic(this, &USP_MoveToTask::OnMoveCompleted);

	// Save RequestID and task infos.
	RequestIDToTaskInfos.Add(Request.MoveId, Infos);

	// Set dynamic: Target actor is already managed by UE.
	Infos->MT_bIsDynamic = bIsDynamic && !Target->IsActor();

	if (PawnSpeed > 0.0f)
	{
		APawn* const AIPawn = Planner.GetPawn();
		SP_RCHECK(AIPawn, false, "AIPawn nullptr! Planner must be attached to a pawn!")

		// Store previous value.
		Infos->MT_PrevPawnSpeed = GetPawnSpeed(AIPawn);

		SetPawnSpeed(AIPawn, PawnSpeed);
	}

	return true;
}
ESP_PlanExecutionState USP_MoveToTask::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, UserData)

	FSP_MoveToTaskInfos* const Infos = reinterpret_cast<FSP_MoveToTaskInfos*>(UserData);

	if (Infos->MT_ExecutionState == ESP_PlanExecutionState::PES_Failed)
	{
		SP_LOG_TASK_TICK(Planner, "Failed")
		return ESP_PlanExecutionState::PES_Failed;
	}
	else if (Infos->MT_ExecutionState == ESP_PlanExecutionState::PES_Succeed)
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
	if (Infos->MT_bIsDynamic)
	{
		USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
		SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

		USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
		SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

		Infos->MT_MoveRequest.SetGoalLocation(Target->GetAnyPosition());

		ASP_AIController* const Controller = Planner.GetController();
		SP_RCHECK_NULLPTR(Controller, ESP_PlanExecutionState::PES_Failed)

		// Request movement.
		#if SP_DEBUG
			FPathFollowingRequestResult Request = Controller->MoveTo(Infos->MT_MoveRequest, &Infos->MT_DebugPath);
		#else
			FPathFollowingRequestResult Request = Controller->MoveTo(Infos->MT_MoveRequest);
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
	FSP_MoveToTaskInfos* Infos = reinterpret_cast<FSP_MoveToTaskInfos*>(UserData);

	if (PawnSpeed > 0.0f)
	{
		APawn* const AIPawn = Planner.GetPawn();
		SP_RCHECK(AIPawn, false, "AIPawn nullptr! Planner must be attached to a pawn!")

		// Reset to saved speed.
		SetPawnSpeed(AIPawn, Infos->MT_PrevPawnSpeed);
	}

	if (Infos->MT_ExecutionState == ESP_PlanExecutionState::PES_Running)
		Infos->MT_Controller->StopMovement(); // Call ReceiveMoveCompleted.

	// Destroy Infos after.
	SP_TASK_SUPER_END(Planner, UserData)

	return true;
}

bool USP_MoveToTask::Cancel(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	FSP_MoveToTaskInfos* Infos = reinterpret_cast<FSP_MoveToTaskInfos*>(UserData);

	if (PawnSpeed > 0.0f)
	{
		APawn* const AIPawn = Planner.GetPawn();
		SP_RCHECK(AIPawn, false, "AIPawn nullptr! Planner must be attached to a pawn!")

		// Reset to saved speed.
		SetPawnSpeed(AIPawn, Infos->MT_PrevPawnSpeed);
	}

	if (Infos->MT_ExecutionState == ESP_PlanExecutionState::PES_Running)
		Infos->MT_Controller->StopMovement(); // Call ReceiveMoveCompleted.

	// Destroy Infos after.
	SP_TASK_SUPER_CANCEL(Planner, UserData)

	return true;
}