// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Decorator/Impl/SP_TimeOutAIDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/SP_FlagHelper.h>
#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>

#include <SPlanner/AI/Decorator/SP_AIPlannerDecoratorFlag.h>

USP_TimeOutAIDecorator::USP_TimeOutAIDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SP_SET_FLAG(ValidateMask, ESP_AIPlannerDecoratorFlag::DF_Tick);

#if WITH_EDITOR
	bCanEditValidateMask = false;
#endif
}

bool USP_TimeOutAIDecorator::Validate_Internal_Implementation(const UObject* Object)
{
	SP_DECORATOR_SUPER_VALIDATE(Object)
	SP_RCHECK_NULLPTR(TimeOutParam, false)

	float TimeOut = TimeOutParam->Query(this);

	if (TimeOut > 0.0f && CurrentTime >= TimeOut)
		return false;

	return true;
}

bool USP_TimeOutAIDecorator::Tick_Validate_Internal_Implementation(float DeltaSeconds,
	const USP_AIPlannerComponent* Planner,
	const USP_TaskInfos* TaskInfos)
{
	CurrentTime += DeltaSeconds;

	return Super::Tick_Validate_Internal_Implementation(DeltaSeconds, Planner, TaskInfos);
}

void USP_TimeOutAIDecorator::OnValidationFailure_Implementation(const UObject* Object)
{
	// Reset for next use.
	CurrentTime = 0.0f;
}