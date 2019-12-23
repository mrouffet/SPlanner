#include <Tasks/Impl/ChooseTargetPositionTask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
#endif

#include <Kismet/KismetMathLibrary.h>

#include <Components/PlannerComponent.h>
#include <Components/TargetComponent.h>

ESP_PlanExecutionState USP_ChooseTargetPositionTask::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	AActor* PlannerOwner = Planner->GetOwner();
	SP_RCHECK_NULLPTR(PlannerOwner, ESP_PlanExecutionState::PES_Failed)


	// Random position with character's Z (only use XY).
	FVector TargetPosition = PlannerOwner->GetActorLocation() + PlannerOwner->GetActorRotation().RotateVector(LocalOffset) +
		FVector(FMath::RandRange(-1.0f, 1.0f) * Dimensions.X, +FMath::RandRange(-1.0f, 1.0f) * Dimensions.Y, 0.0f);

	Planner->GetTarget()->SetPosition(TargetPosition);
		

#if SP_DEBUG
	if (PlannerOwner->IsSelectedInEditor())
		DrawDebugLine(PlannerOwner->GetWorld(), PlannerOwner->GetActorLocation(), TargetPosition, FColor::Cyan, false, USP_Settings::GetDebugScreenDisplayTime() / 2.0f);

	SP_LOG_EXECUTE(PlannerOwner, "%s", *TargetPosition.ToString())
#endif


	if (bAutoLookAt)
		PlannerOwner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlannerOwner->GetActorLocation(), TargetPosition));

	return ESP_PlanExecutionState::PES_Succeed;
}