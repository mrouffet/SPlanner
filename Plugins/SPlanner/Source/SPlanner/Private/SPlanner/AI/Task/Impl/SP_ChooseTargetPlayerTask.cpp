// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/SP_ChooseTargetPlayerTask.h>

#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Blackboard/Object/Target/SP_Target.h>

bool USP_ChooseTargetPlayerTask::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->AddBlackboardFlag(OutTargetEntryName, ESP_AIBBPlanGenFlags::PG_TargetPlayer);

	return true;
}
bool USP_ChooseTargetPlayerTask::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->RemoveBlackboardFlag(OutTargetEntryName, ESP_AIBBPlanGenFlags::PG_TargetPlayer);

	return true;
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

	USP_Target* const OutTarget = Blackboard->GetObject<USP_Target>(OutTargetEntryName);
	SP_RCHECK_NULLPTR(OutTarget, ESP_PlanExecutionState::PES_Failed)

	OutTarget->SetPlayer(TargetPlayer);

#if SP_DEBUG_EDITOR

	DrawDebug(Planner, TargetPlayer->GetActorLocation());

	SP_LOG_TASK_EXECUTE(Planner, "%s", *TargetPlayer->GetName())

#endif

	return ESP_PlanExecutionState::PES_Succeed;
}