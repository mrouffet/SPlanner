// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_TargetNearestPlayerTask.h>

#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

bool USP_TargetNearestPlayerTask::Predicate_Implementation(USP_AIPlannerComponent* Planner, APawn* Player) const
{
	SP_RCHECK_NULLPTR(Player, false)

	return true;
}

bool USP_TargetNearestPlayerTask::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

#if SP_DEBUG
	// Check valid blackboard entry.
	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)
#endif

	// Not already re-targeted.
	return !SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty);
}
uint64 USP_TargetNearestPlayerTask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetActor);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPlayer);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_TargetNearestPlayerTask::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_TICK_SUPER(DeltaSeconds, Planner, UserData)

	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	AGameStateBase* GameState = Planner.GetWorld()->GetGameState();

	SP_RCHECK_NULLPTR(GameState, ESP_PlanExecutionState::PES_Failed)

	float CurrSqrDist = FLT_MAX;
	APawn* NearestPlayer = nullptr;

	FVector PlannerLocation = Planner.GetOwner()->GetActorLocation();

	for(int i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(GameState->PlayerArray[i])
		SP_CCHECK_NULLPTR(GameState->PlayerArray[i]->GetPawn())

		APawn* const Player = GameState->PlayerArray[i]->GetPawn();

		// must validate predicate.
		if (!Predicate(&Planner, Player))
			continue;

		float SqrDist = FVector::DistSquared(PlannerLocation, Player->GetActorLocation());

		if (SqrDist < CurrSqrDist)
		{
			CurrSqrDist = SqrDist;
			NearestPlayer = Player;
		}
	}

	if(NearestPlayer == nullptr)
		return ESP_PlanExecutionState::PES_Failed;

	Target->SetPlayer(NearestPlayer);

	return ESP_PlanExecutionState::PES_Succeed;
}