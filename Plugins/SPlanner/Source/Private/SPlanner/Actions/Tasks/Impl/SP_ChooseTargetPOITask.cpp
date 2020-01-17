#include <SPlanner/Actions/Tasks/Impl/SP_ChooseTargetPOITask.h>

#if SP_DEBUG
#include <DrawDebugHelpers.h>
#endif

#include <SPlanner/Components/SP_POIComponent.h>
#include <SPlanner/Components/SP_TargetComponent.h>
#include <SPlanner/Components/Planners/SP_AIPlannerComponent.h>

FSP_PlannerFlags USP_ChooseTargetPOITask::PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_POI);
	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Actor);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_ChooseTargetPOITask::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	AActor* TargetOwner = Planner->Target->GetOwner();
	SP_RCHECK_NULLPTR(TargetOwner, ESP_PlanExecutionState::PES_Failed)

	FCollisionQueryParams QParams;
	QParams.AddIgnoredActor(TargetOwner);

	FVector Start = TargetOwner->GetActorLocation() + TargetOwner->GetActorRotation().RotateVector(LocalOffset);

	TArray<FHitResult> HitInfos;
	Planner->GetWorld()->SweepMultiByChannel(
		HitInfos,
		Start,
		Start + FVector(0.0f, 0.0f, 0.1f),
		TargetOwner->GetActorRotation().Quaternion(),
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeBox(Dimensions / 2.0f),
		QParams
	);

	TArray<USP_POIComponent*> POIs;

	for (int i = 0; i < HitInfos.Num(); ++i)
	{
		if (USP_POIComponent* POI = Cast<USP_POIComponent>(HitInfos[i].Component))
			POIs.Add(POI);
	}

	// No POI found.
	if(POIs.Num() == 0)
		return ESP_PlanExecutionState::PES_Failed;

#if SP_DEBUG_EDITOR

	USP_POIComponent* TargetPOI = POIs[FMath::RandRange(0, POIs.Num() - 1)];
	Planner->Target->SetPOI(TargetPOI);

	SP_IF_TASK_EXECUTE(Planner->GetOwner())
	{
		DrawDebugSphere(TargetOwner->GetWorld(),
			TargetOwner->GetActorLocation() + TargetOwner->GetActorRotation().RotateVector(LocalOffset),
			Dimensions.X > Dimensions.Y ? Dimensions.X : Dimensions.Y,
			25, DebugColor, false,
			USP_Settings::GetDebugScreenDisplayTime() / 2.0f);

		DrawDebugLine(TargetOwner->GetWorld(), TargetOwner->GetActorLocation(), TargetPOI->GetOwner()->GetActorLocation(), DebugColor, false, DebugDrawTime);
	}

	SP_LOG_TASK_EXECUTE(Planner->GetOwner(), "%s", *TargetPOI->GetOwner()->GetName())

#else

	Planner->Target->SetPOI(POIs[FMath::RandRange(0, POIs.Num() - 1)]);

#endif

	return ESP_PlanExecutionState::PES_Succeed;
}