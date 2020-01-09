#include <Tasks/Impl/SP_ChooseTargetPOITask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
#endif

#include <Components/SP_POIComponent.h>
#include <Components/SP_TargetComponent.h>
#include <Components/SP_PlannerComponent.h>

FSP_PlannerFlags USP_ChooseTargetPOITask::PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_TASK_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_POI);
	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Actor);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_ChooseTargetPOITask::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	AActor* PlannerOwner = Planner->GetOwner();
	SP_RCHECK_NULLPTR(PlannerOwner, ESP_PlanExecutionState::PES_Failed)

	FCollisionQueryParams QParams;
	QParams.AddIgnoredActor(PlannerOwner);

	FVector Start = PlannerOwner->GetActorLocation() + PlannerOwner->GetActorRotation().RotateVector(LocalOffset);

	TArray<FHitResult> HitInfos;
	Planner->GetWorld()->SweepMultiByChannel(
		HitInfos,
		Start,
		Start + FVector(0.0f, 0.0f, 0.1f),
		PlannerOwner->GetActorRotation().Quaternion(),
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

	SP_IF_TASK_EXECUTE(PlannerOwner)
		DrawDebugLine(PlannerOwner->GetWorld(), PlannerOwner->GetActorLocation(), TargetPOI->GetOwner()->GetActorLocation(), FColor::Cyan, false, USP_Settings::GetDebugScreenDisplayTime() / 2.0f);

	SP_LOG_TASK_EXECUTE(PlannerOwner, "%s", *TargetPOI->GetOwner()->GetName())

#else

	Planner->GetTarget()->SetPOI(POIs[FMath::RandRange(0, POIs.Num() - 1)]);

#endif

	return ESP_PlanExecutionState::PES_Succeed;
}