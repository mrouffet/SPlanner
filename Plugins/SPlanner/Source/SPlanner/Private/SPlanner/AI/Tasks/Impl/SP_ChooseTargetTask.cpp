// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>

#if SP_DEBUG
	#include <DrawDebugHelpers.h>
#endif

#include <SPlanner/AI/Target/SP_Target.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

FVector USP_ChooseTargetTask::GetFOVCenter(const APawn* Pawn) const
{
	SP_RCHECK_NULLPTR(Pawn, FVector::ZeroVector)

	return Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(LocalOffset);
}

bool USP_ChooseTargetTask::Predicate_Implementation(const USP_AIPlannerComponent* Planner, const AActor* Actor) const
{
	SP_RCHECK_NULLPTR(Actor, false)
	SP_RCHECK_NULLPTR(Planner, false)

	APawn* const Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, false)

	FVector FOVCenter = Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(LocalOffset);
	FVector FOVHalfDimensions = Pawn->GetActorRotation().RotateVector(HalfDimensions);

	FVector FOVToTarget = Actor->GetActorLocation() - FOVCenter;

	// Out of X bound.
	if (HalfDimensions.X > 0.0f && FMath::Abs(FOVToTarget.X) > FOVHalfDimensions.X)
		return false;

	// Out of Y bound.
	if (HalfDimensions.Y > 0.0f && FMath::Abs(FOVToTarget.Y) > FOVHalfDimensions.Y)
		return false;

	// Out of Z bound.
	if (HalfDimensions.Z > 0.0f && FMath::Abs(FOVToTarget.Z) > FOVHalfDimensions.Z)
		return false;


	if (bTargetVisible)
	{
		FHitResult HitInfos;
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(Pawn);
		Params.AddIgnoredActor(Actor);

		return !Planner->GetWorld()->LineTraceSingleByChannel(HitInfos,
			FOVCenter,
			Actor->GetActorLocation(),
			ECollisionChannel::ECC_Visibility,
			Params);
	}

	return true;
}
AActor* USP_ChooseTargetTask::Choose_Implementation(const USP_AIPlannerComponent* Planner, const TArray<AActor*>& Actors)
{
	SP_RCHECK_NULLPTR(Planner, nullptr)

	switch (Method)
	{
	case ESP_ChooseTargetMethod::CTM_Nearest:
	{
		float ClosestSqrDist = FLT_MAX;
		AActor* Target = nullptr;
		FVector PlannerLocation = Planner->GetOwner()->GetActorLocation();

		for (int i = 0; i < Actors.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(Actors[i])

			float SqrDist = FVector::DistSquared(PlannerLocation, Actors[i]->GetActorLocation());

			if (SqrDist < ClosestSqrDist)
			{
				SqrDist = ClosestSqrDist;
				Target = Actors[i];
			}
		}

		return Target;
	}
	case ESP_ChooseTargetMethod::CTM_Furthest:
	{
		float FurthestSqrDist = FLT_MAX;
		AActor* Target = nullptr;
		FVector PlannerLocation = Planner->GetOwner()->GetActorLocation();

		for (int i = 0; i < Actors.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(Actors[i])

			float SqrDist = FVector::DistSquared(PlannerLocation, Actors[i]->GetActorLocation());

			if (SqrDist > FurthestSqrDist)
			{
				SqrDist = FurthestSqrDist;
				Target = Actors[i];
			}
		}

		return Target;
	}
	default:
		SP_LOG(Warning, "ChooseTargetMethod not supported yet!")
	case ESP_ChooseTargetMethod::CTM_Random:
	{
		// Empty Targets.
		if (!Actors.Num())
			return nullptr;

		return Actors[FMath::RandRange(0, Actors.Num() - 1)];
	}
	}
}

bool USP_ChooseTargetTask::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

#if SP_DEBUG
	// Check valid blackboard entry.
	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)
#endif

	// Not already re-targeted.
	if(!bAllowReTarget && SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty))
		return false;

	return true;
}
uint64 USP_ChooseTargetTask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPosition);

	return PlannerFlags;
}

#if SP_DEBUG_EDITOR
void USP_ChooseTargetTask::DrawDebug(const USP_AIPlannerComponent* Planner, const FVector& TargetLocation)
{
	APawn* const Pawn = Planner->GetPawn();
	SP_CHECK_NULLPTR(Pawn)

	FVector FOVCenter = GetFOVCenter(Pawn);

	SP_IF_TASK_EXECUTE(Planner)
	{
		DrawDebugBox(Pawn->GetWorld(),
			FOVCenter,
			HalfDimensions * 2.0f,
			DebugColor, false,
			DebugDrawTime);

		DrawDebugLine(Pawn->GetWorld(),
			FOVCenter,
			TargetLocation,
			DebugColor, false,
			DebugDrawTime);
	}
}
#endif