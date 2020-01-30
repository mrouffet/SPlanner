#include <SPlanner/Actions/AITasks/Impl/SP_ChooseTargetPOITask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
#endif

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/Miscs/Flags/SP_AIPlannerFlags.h>

#include <SPlanner/Components/SP_POIComponent.h>
#include <SPlanner/Components/SP_TargetComponent.h>
#include <SPlanner/Components/Planners/SP_AIPlannerComponent.h>

bool USP_ChooseTargetPOITask::Predicate_Implementation(const USP_AIPlannerComponent* Planner, const USP_POIComponent* TargetPOI) const
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(TargetPOI, false)

	return true;
}

uint64 USP_ChooseTargetPOITask::PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPOI);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetActor);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_ChooseTargetPOITask::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_AI_TASK_TICK_SUPER(DeltaSeconds, Planner, UserData)

	AActor* const TargetOwner = Planner->Target->GetOwner();
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
		if (USP_POIComponent* const POI = Cast<USP_POIComponent>(HitInfos[i].Component))
		{
			UClass* POIActorClass = POI->GetOwner()->GetClass();

			SP_CCHECK(AllowedPOIActorTypes.Find(POIActorClass) != INDEX_NONE &&
				IgnoredPOIActorTypes.Find(POIActorClass) != INDEX_NONE,
				"POI Actor type [%s] is both allowed and ignored!", *POIActorClass->GetName())

			// Allowed or not ignored if no allowed.
			if (IgnoredPOIActorTypes.Num() && IgnoredPOIActorTypes.Find(POIActorClass) != INDEX_NONE ||
				AllowedPOIActorTypes.Num() && AllowedPOIActorTypes.Find(POIActorClass) == INDEX_NONE)
				continue;

			// Match overridden predicate.
			if(Predicate(Planner, POI))
				POIs.Add(POI);
		}
	}

	// No POI found.
	if(POIs.Num() == 0)
		return ESP_PlanExecutionState::PES_Failed;

	USP_POIComponent* TargetPOI = nullptr;

	if (bTargetNearest)
	{
		float ClosestSqrDist = FLT_MAX;
		FVector PlannerLocation = Planner->GetOwner()->GetActorLocation();

		for (int i = 0; i < POIs.Num(); ++i)
		{
			float SqrDist = FVector::DistSquared(PlannerLocation, POIs[i]->GetComponentLocation());

			if (SqrDist < ClosestSqrDist)
			{
				SqrDist = ClosestSqrDist;
				TargetPOI = POIs[i];
			}
		}
	}
	else // Choose random one.
		TargetPOI = POIs[FMath::RandRange(0, POIs.Num() - 1)];
	
	Planner->Target->SetPOI(TargetPOI);

#if SP_DEBUG_EDITOR

	SP_IF_AI_TASK_EXECUTE(Planner)
	{
		DrawDebugSphere(TargetOwner->GetWorld(),
			TargetOwner->GetActorLocation() + TargetOwner->GetActorRotation().RotateVector(LocalOffset),
			Dimensions.X > Dimensions.Y ? Dimensions.X : Dimensions.Y,
			25, DebugColor, false,
			USP_Settings::GetDebugScreenDisplayTime() / 2.0f);

		DrawDebugLine(TargetOwner->GetWorld(), TargetOwner->GetActorLocation(), TargetPOI->GetOwner()->GetActorLocation(), DebugColor, false, DebugDrawTime);
	}

	SP_LOG_AI_TASK_EXECUTE(Planner, "%s", *TargetPOI->GetOwner()->GetName())

#endif

	if (bAutoLookAt)
	{
		// Only Yaw rotation.
		FVector TargetPosition = TargetPOI->GetOwner()->GetActorLocation();
		TargetPosition.Z = TargetOwner->GetActorLocation().Z;

		TargetOwner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(TargetOwner->GetActorLocation(), TargetPosition));
	}

	return ESP_PlanExecutionState::PES_Succeed;
}