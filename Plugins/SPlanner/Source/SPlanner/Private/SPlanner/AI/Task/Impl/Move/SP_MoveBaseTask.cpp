// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/Move/SP_MoveBaseTask.h>

#include <Components/CapsuleComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

float USP_MoveBaseTask::GetPawnSpeed_Implementation(APawn* Pawn)
{
	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		UCharacterMovementComponent* const CharacterMovement = Character->GetCharacterMovement();
		SP_RCHECK_NULLPTR(CharacterMovement, false)

		return CharacterMovement->MaxWalkSpeed;
	}

	return -1.0f;
}

bool USP_MoveBaseTask::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	// Not already moved to.
	if(!bAllowMultipleMove && AIPlanGenInfos->IsFlagSet(ESP_AIPlanGenFlags::PG_PawnDirtyLocation))
		return false;

	// New target will be set.
	if (AIPlanGenInfos->IsBlackboardFlagSet(TargetEntryName, ESP_AIBBPlanGenFlags::PG_Dirty))
		return true;

	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Infos->Planner);

	USP_AIBlackboardComponent* const Blackboard = AIPlanner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	// Get Target.
	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	// Check valid target.
	return Target->IsValid();
}
bool USP_MoveBaseTask::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)
	
	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->AddFlags(ESP_AIPlanGenFlags::PG_PawnDirtyLocation, ESP_AIPlanGenFlags::PG_PawnDirtyRotation);

	return true;
}
bool USP_MoveBaseTask::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)
	
	AIPlanGenInfos->RemoveFlags(ESP_AIPlanGenFlags::PG_PawnDirtyLocation, ESP_AIPlanGenFlags::PG_PawnDirtyRotation);

	return true;
}