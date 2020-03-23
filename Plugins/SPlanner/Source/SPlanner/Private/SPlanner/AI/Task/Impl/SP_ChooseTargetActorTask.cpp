// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/SP_ChooseTargetActorTask.h>

#include <SPlanner/AI/Blackboard/Object/Target/SP_Target.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

bool USP_ChooseTargetActorTask::Predicate_Implementation(const USP_AIPlannerComponent* Planner, const AActor* Actor) const
{
	SP_RCHECK_NULLPTR(Actor, false)
	SP_RCHECK_NULLPTR(Planner, false)

	const APawn* const Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, false)

	const FVector ActorLocation = Actor->GetActorLocation();

	if (IsInMinBox(Pawn, ActorLocation) || !IsInMaxBox(Pawn, ActorLocation))
		return false;

	if (bTargetVisible)
	{
		FHitResult HitInfos;
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(Pawn);
		Params.AddIgnoredActor(Actor);

		return !Planner->GetWorld()->LineTraceSingleByChannel(HitInfos,
			GetFOVCenter(Pawn),
			Actor->GetActorLocation(),
			ECollisionChannel::ECC_Visibility,
			Params);
	}

	return true;
}
AActor* USP_ChooseTargetActorTask::Choose_Implementation(const USP_AIPlannerComponent* Planner, const TArray<AActor*>& Actors)
{
	SP_RCHECK_NULLPTR(Planner, nullptr)

	switch (Method)
	{
	case ESP_ChooseTargetActorMethod::CTM_Nearest:
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
	case ESP_ChooseTargetActorMethod::CTM_Furthest:
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
		SP_LOG(Warning, "ChooseTargetActorMethod not supported yet!")
	case ESP_ChooseTargetActorMethod::CTM_Random:
	{
		// Empty Targets.
		if (!Actors.Num())
			return nullptr;

		return Actors[FMath::RandRange(0, Actors.Num() - 1)];
	}
	}
}

bool USP_ChooseTargetActorTask::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->AddBlackboardFlag(OutTargetEntryName, ESP_AIBBPlanGenFlags::PG_TargetActor);

	return true;
}
bool USP_ChooseTargetActorTask::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->RemoveBlackboardFlag(OutTargetEntryName, ESP_AIBBPlanGenFlags::PG_TargetActor);

	return true;
}