// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Decorator/Impl/SP_BBEntryValidAIDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>
#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey.h>

bool USP_BBEntryValidAIDecorator::Validate_Internal_Implementation(const UObject* Object)
{
	SP_DECORATOR_SUPER_VALIDATE(Object)

	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Object);
	SP_RCHECK(AIPlanner, false, "AIPlanner nullptr! Object must be of type USP_AIPlannerComponent!")

	const USP_AIBlackboardComponent* const Blackboard = AIPlanner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_AIBlackboardKey* BBKey = Blackboard->GetKey(EntryName);
	SP_RCHECK(BBKey, false, "Key %s not registered in Blackboard!", *EntryName.ToString())

	return BBKey->IsValidEntry();
}