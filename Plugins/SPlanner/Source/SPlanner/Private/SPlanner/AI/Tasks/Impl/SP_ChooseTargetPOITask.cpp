// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetPOITask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
#endif

#include <Kismet/KismetMathLibrary.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_BlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>
#include <SPlanner/AI/POI/SP_POIComponent.h>

bool USP_ChooseTargetPOITask::Predicate_Implementation(const USP_AIPlannerComponent* Planner, const USP_POIComponent* TargetPOI) const
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(TargetPOI, false)

	return true;
}
USP_POIComponent* USP_ChooseTargetPOITask::ChoosePOI_Implementation(const USP_AIPlannerComponent* Planner, const TArray<USP_POIComponent*>& POIs)
{
	SP_RCHECK_NULLPTR(Planner, nullptr)

	// Choose random one.
	if(!bTargetNearest)
		return POIs[FMath::RandRange(0, POIs.Num() - 1)];

	float ClosestSqrDist = FLT_MAX;
	USP_POIComponent* TargetPOI = nullptr;
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

	return TargetPOI;
}

uint64 USP_ChooseTargetPOITask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPOI);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetActor);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_ChooseTargetPOITask::Tick_Internal(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	// Do not use SP_TASK_TICK_SUPER macro (require Super::Tick_Internal and not Super::Tick call).
	ESP_PlanExecutionState SuperInternalResult = Super::Tick_Internal(DeltaSeconds, Planner, UserData);
	if (SuperInternalResult != ESP_PlanExecutionState::PES_Succeed)
		return SuperInternalResult;

	USP_BlackboardComponent* const Blackboard = Planner.GetBlackboard();
	SP_RCHECK_NULLPTR(Blackboard, ESP_PlanExecutionState::PES_Failed)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	APawn* const Pawn = Planner.GetPawn();
	SP_RCHECK_NULLPTR(Pawn, ESP_PlanExecutionState::PES_Failed)

	FCollisionQueryParams QParams;
	QParams.AddIgnoredActor(Pawn);

	FVector Start = Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(LocalOffset);

	TArray<FHitResult> HitInfos;
	Planner.GetWorld()->SweepMultiByChannel(
		HitInfos,
		Start,
		Start + FVector(0.0f, 0.0f, 0.1f),
		Pawn->GetActorRotation().Quaternion(),
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

			SP_CCHECK(AllowedPOIActorTypes.Find(POIActorClass) == INDEX_NONE ||
				IgnoredPOIActorTypes.Find(POIActorClass) == INDEX_NONE,
				"POI Actor type [%s] is both allowed and ignored!", *POIActorClass->GetName())

			// Allowed or not ignored if no allowed.
			if ((IgnoredPOIActorTypes.Num() && IgnoredPOIActorTypes.Find(POIActorClass) != INDEX_NONE) ||
				(AllowedPOIActorTypes.Num() && AllowedPOIActorTypes.Find(POIActorClass) == INDEX_NONE))
				continue;

			// Match overridden predicate.
			if(Predicate(&Planner, POI))
				POIs.Add(POI);
		}
	}

	// No POI found.
	if(POIs.Num() == 0)
		return ESP_PlanExecutionState::PES_Failed;

	USP_POIComponent* TargetPOI = ChoosePOI(&Planner, POIs);
	
	Target->SetPOI(TargetPOI);

#if SP_DEBUG_EDITOR

	SP_IF_TASK_EXECUTE(Planner)
	{
		DrawDebugSphere(Pawn->GetWorld(),
			Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(LocalOffset),
			Dimensions.X > Dimensions.Y ? Dimensions.X : Dimensions.Y,
			25, DebugColor, false,
			USP_EditorSettings::GetDebugScreenDisplayTime() / 2.0f);

		DrawDebugLine(Pawn->GetWorld(), Pawn->GetActorLocation(), TargetPOI->GetOwner()->GetActorLocation(), DebugColor, false, DebugDrawTime);
	}

	SP_LOG_TASK_EXECUTE(Planner, "%s", *TargetPOI->GetOwner()->GetName())

#endif

	return ESP_PlanExecutionState::PES_Succeed;
}