// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Decorator/Impl/SP_TimeOutAIDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIFloatParam.h>

#include <SPlanner/AI/Decorator/SP_AIPlannerDecoratorFlag.h>

USP_TimeOutAIDecorator::USP_TimeOutAIDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TimeOutParam = CreateDefaultSubobject<USP_AIFloatParam>("TimeOutParam");
	TimeOutParam->DefaultValue = 5.0f;

	ValidateMask = static_cast<uint8>(ESP_AIPlannerDecoratorFlag::DF_Tick);
}

bool USP_TimeOutAIDecorator::Validate_Internal_Implementation(const UObject* Object)
{
	SP_DECORATOR_SUPER_VALIDATE(Object)

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

#if WITH_EDITOR
void USP_TimeOutAIDecorator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Force PreCondition only. 
	if (PropertyChangedEvent.GetPropertyName() == "ValidateMask")
	{
		SP_LOG(Warning, "Decorator must always be checked in Tick.")
		ValidateMask = static_cast<uint8>(ESP_AIPlannerDecoratorFlag::DF_Tick);
	}
}
#endif