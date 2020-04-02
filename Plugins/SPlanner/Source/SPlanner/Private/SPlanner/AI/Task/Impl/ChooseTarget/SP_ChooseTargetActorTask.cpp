// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetActorTask.h>

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

	if ((HasValidMinBox() && IsInMinBox(Pawn, ActorLocation)) || (HasValidMaxBox() && !IsInMaxBox(Pawn, ActorLocation)))
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

	const APawn* const Pawn = Planner->GetPawn();
	SP_RCHECK_NULLPTR(Planner, nullptr)

	const FVector PawnLocation = Pawn->GetActorLocation();

	switch (Method)
	{
		case ESP_ChooseTargetActorMethod::CTM_Nearest:
		{
			AActor* Target = nullptr;
			float ClosestSqrDist = FLT_MAX;

			for (int i = 0; i < Actors.Num(); ++i)
			{
				SP_CCHECK_NULLPTR(Actors[i])

				float SqrDist = FVector::DistSquared(PawnLocation, Actors[i]->GetActorLocation());

				if (SqrDist < ClosestSqrDist)
				{
					ClosestSqrDist = SqrDist;
					Target = Actors[i];
				}
			}

			return Target;
		}
		case ESP_ChooseTargetActorMethod::CTM_Furthest:
		{
			AActor* Target = nullptr;
			float FurthestSqrDist = FLT_MAX;

			for (int i = 0; i < Actors.Num(); ++i)
			{
				SP_CCHECK_NULLPTR(Actors[i])

				float SqrDist = FVector::DistSquared(PawnLocation, Actors[i]->GetActorLocation());

				if (SqrDist > FurthestSqrDist)
				{
					FurthestSqrDist = SqrDist;
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