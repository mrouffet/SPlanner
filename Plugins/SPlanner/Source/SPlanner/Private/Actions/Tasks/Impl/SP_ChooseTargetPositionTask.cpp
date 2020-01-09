#include <Actions/Tasks/Impl/SP_ChooseTargetPositionTask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
#endif

#include <Kismet/KismetMathLibrary.h>

#include <Components/Planners/SP_AIPlannerComponent.h>
#include <Components/SP_TargetComponent.h>

ESP_PlanExecutionState USP_ChooseTargetPositionTask::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	AActor* TargetOwner = Planner->Target->GetOwner();
	SP_RCHECK_NULLPTR(TargetOwner, ESP_PlanExecutionState::PES_Failed)


	// Random position with character's Z (only use XY).
	FVector TargetPosition = TargetOwner->GetActorLocation() + TargetOwner->GetActorRotation().RotateVector(LocalOffset) +
		FVector(FMath::RandRange(-1.0f, 1.0f) * Dimensions.X, +FMath::RandRange(-1.0f, 1.0f) * Dimensions.Y, 0.0f);

	Planner->Target->SetPosition(TargetPosition);
		

#if SP_DEBUG_EDITOR
	SP_IF_TASK_EXECUTE(Planner->GetOwner())
	{
		DrawDebugSphere(TargetOwner->GetWorld(),
			TargetOwner->GetActorLocation() + TargetOwner->GetActorRotation().RotateVector(LocalOffset),
			Dimensions.X > Dimensions.Y ? Dimensions.X : Dimensions.Y,
			25, DebugColor, false,
			USP_Settings::GetDebugScreenDisplayTime() / 2.0f);

		DrawDebugLine(TargetOwner->GetWorld(), TargetOwner->GetActorLocation(), TargetPosition, DebugColor, false, DebugDrawTime);
	}

	SP_LOG_TASK_EXECUTE(Planner->GetOwner(), "%s", *TargetPosition.ToString())
#endif


	if (bAutoLookAt)
		TargetOwner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(TargetOwner->GetActorLocation(), TargetPosition));

	return ESP_PlanExecutionState::PES_Succeed;
}