#include <Actions/Tasks/Impl/SP_TargetNearestPlayerTask.h>

#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>

#include <Components/Planners/SP_AIPlannerComponent.h>
#include <Components/SP_TargetComponent.h>

bool USP_TargetNearestPlayerTask::PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, PlannerFlags)

	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);

	// Valid Target component.
	SP_RCHECK_NULLPTR(AIPlanner, false)
	SP_RCHECK_NULLPTR(AIPlanner->Target, false)

	// Not already re-targeted.
	return !SP_IS_FLAG_SET(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Dirty);
}
FSP_PlannerFlags USP_TargetNearestPlayerTask::PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Dirty);
	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Actor);
	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Player);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_TargetNearestPlayerTask::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	// Valid Target component.
	SP_RCHECK_NULLPTR(Planner->Target, ESP_PlanExecutionState::PES_Failed)

	AGameStateBase* GameState = Planner->GetWorld()->GetGameState();

	SP_RCHECK_NULLPTR(GameState, ESP_PlanExecutionState::PES_Failed)

	float CurrSqrDist = FLT_MAX;
	APawn* NearestPlayer = nullptr;

	FVector PlannerLocation = Planner->GetOwner()->GetActorLocation();

	for(int i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(GameState->PlayerArray[i], ESP_PlanExecutionState::PES_Failed)
		SP_RCHECK_NULLPTR(GameState->PlayerArray[i]->GetPawn(), ESP_PlanExecutionState::PES_Failed)

		float SqrDist = FVector::DistSquared(PlannerLocation, GameState->PlayerArray[i]->GetPawn()->GetActorLocation());

		if (SqrDist < CurrSqrDist)
		{
			CurrSqrDist = SqrDist;
			NearestPlayer = GameState->PlayerArray[i]->GetPawn();
		}
	}

	if(NearestPlayer == nullptr)
		return ESP_PlanExecutionState::PES_Failed;

	Planner->Target->SetPlayer(NearestPlayer);

	return ESP_PlanExecutionState::PES_Succeed;
}