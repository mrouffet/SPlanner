// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Decorator/Impl/SP_IsVisibleAIDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>
#include <SPlanner/AI/Blackboard/Object/Target/SP_Target.h>

#include <SPlanner/AI/Decorator/SP_AIPlannerDecoratorFlag.h>


USP_IsVisibleAIDecorator::USP_IsVisibleAIDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SP_SET_FLAG(ValidateMask, ESP_PlannerDecoratorFlag::DF_PreCondition);
	SP_ADD_FLAG(ValidateMask, ESP_AIPlannerDecoratorFlag::DF_Begin);
}

bool USP_IsVisibleAIDecorator::Validate_Internal_Implementation(const UObject* Object)
{
	SP_DECORATOR_SUPER_VALIDATE(Object)

	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Object);
	SP_RCHECK(AIPlanner, false, "AIPlanner nullptr! Object must be of type USP_AIPlannerComponent!")

	const APawn* const Pawn = AIPlanner->GetPawn();
	SP_RCHECK_NULLPTR(Pawn, false)

	const USP_AIBlackboardComponent* const Blackboard = AIPlanner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	FVector TargetPos;
	FHitResult HitInfos;
	FCollisionQueryParams Params;

	Params.AddIgnoredActor(Pawn);

	if (const USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName))
	{
		if (const AActor* TargetActor = Target->GetActor())
			Params.AddIgnoredActor(TargetActor);

		TargetPos = Target->GetAnyPosition();
	}
	else
		TargetPos = Blackboard->GetVector(TargetEntryName);

	return !AIPlanner->GetWorld()->LineTraceSingleByChannel(HitInfos,
		Pawn->GetActorLocation(),
		TargetPos,
		ECollisionChannel::ECC_Visibility,
		Params);
}

bool USP_IsVisibleAIDecorator::PreCondition_Validate_Internal_Implementation(const USP_PlanGenInfos* Infos)
{
	const USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK(AIPlanGenInfos, false, "AIPlanGenInfos nullptr! Infos must be of type USP_AIPlanGenInfos!")

	// Target is dirty and can't be checked: predict true.
	if (AIPlanGenInfos->IsBlackboardFlagSet(TargetEntryName, ESP_AIBBPlanGenFlags::PG_Dirty))
		return true;

	return Super::PreCondition_Validate_Internal_Implementation(Infos);
}