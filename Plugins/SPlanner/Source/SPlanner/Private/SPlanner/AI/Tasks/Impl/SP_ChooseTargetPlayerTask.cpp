// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetPlayerTask.h>

#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

uint64 USP_ChooseTargetPlayerTask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetActor);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPlayer);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_ChooseTargetPlayerTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	AGameStateBase* const GameState = Planner->GetWorld()->GetGameState();
	SP_RCHECK_NULLPTR(GameState, ESP_PlanExecutionState::PES_Failed)

	TArray<AActor*> AvailablePlayers;

	for (int i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(GameState->PlayerArray[i])

		APawn* const PlayerPawn = GameState->PlayerArray[i]->GetPawn();
		SP_CCHECK_NULLPTR(PlayerPawn)

		if (Predicate(Planner, PlayerPawn))
			AvailablePlayers.Add(PlayerPawn);
	}

	// No Available players.
	if(!AvailablePlayers.Num())
		return ESP_PlanExecutionState::PES_Failed;

	APawn* const TargetPlayer = Cast<APawn>(Choose(Planner, AvailablePlayers));

	// No Target player found.
	if(!TargetPlayer)
		return ESP_PlanExecutionState::PES_Failed;


	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	Target->SetPlayer(TargetPlayer);

#if SP_DEBUG_EDITOR

	DrawDebug(Planner, TargetPlayer->GetActorLocation());

	SP_LOG_TASK_EXECUTE(Planner, "%s", *TargetPlayer->GetName())

#endif

	return ESP_PlanExecutionState::PES_Succeed;
}