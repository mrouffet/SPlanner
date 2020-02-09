#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetPositionTask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
#endif

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/AI/Blackboard/SP_BlackboardComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

ESP_PlanExecutionState USP_ChooseTargetPositionTask::Tick_Internal(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	// Do not use SP_TASK_TICK_SUPER macro (require Super::Tick_Internal and not Super::Tick call).
	ESP_PlanExecutionState SuperInternalResult = Super::Tick_Internal(DeltaSeconds, Planner, UserData);
	if (SuperInternalResult != ESP_PlanExecutionState::PES_Succeed)
		return SuperInternalResult;

	USP_BlackboardComponent* const Blackboard = Planner.GetBlackboard();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	APawn* Pawn = Planner.GetPawn();
	SP_RCHECK_NULLPTR(Pawn, ESP_PlanExecutionState::PES_Failed)


	// Random position with character's Z (only use XY).
	FVector TargetPosition = Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(LocalOffset) +
		FVector(FMath::RandRange(-1.0f, 1.0f) * Dimensions.X, FMath::RandRange(-1.0f, 1.0f) * Dimensions.Y, 0.0f);

	Target->SetPosition(TargetPosition);

#if SP_DEBUG_EDITOR
	SP_IF_TASK_EXECUTE(Planner)
	{
		DrawDebugSphere(Pawn->GetWorld(),
			Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(LocalOffset),
			Dimensions.X > Dimensions.Y ? Dimensions.X : Dimensions.Y,
			25, DebugColor, false,
			USP_Settings::GetDebugScreenDisplayTime() / 2.0f);

		DrawDebugLine(Pawn->GetWorld(), Pawn->GetActorLocation(), TargetPosition, DebugColor, false, DebugDrawTime);
	}

	SP_LOG_TASK_EXECUTE(Planner, "%s", *TargetPosition.ToString())
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}