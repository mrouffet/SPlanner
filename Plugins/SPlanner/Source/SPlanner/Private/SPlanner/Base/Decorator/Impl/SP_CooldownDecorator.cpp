// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/Impl/SP_CooldownDecorator.h>

#include <SPlanner/Misc/VariableParam/Float/SP_FloatParam.h>

void USP_CooldownDecorator::SetCooldown(const UObject* Object)
{
	float PlannerCoolDown = Cooldown->Query(Object);

	// Never save no cooldown.
	if (PlannerCoolDown <= 0.0f)
		return;

	// Add 0.001f to ensure float precision.
	Cooldowns.FindOrAdd(Object) = Object->GetWorld()->GetTimeSeconds() + PlannerCoolDown + 0.001f;
}

bool USP_CooldownDecorator::Validate_Internal_Implementation(const UObject* Object)
{
	SP_DECORATOR_SUPER_VALIDATE(Object)

	const float* const CooldownPtr = Cooldowns.Find(Object);

	float ObjectCoolDown = CooldownPtr ? *CooldownPtr - Object->GetWorld()->GetTimeSeconds() : -1.0f;

	return ObjectCoolDown <= 0.0f;
}

void USP_CooldownDecorator::PostExecution_Internal_Implementation(const UObject* Object, bool bExecutionSuccess)
{
	Super::PostExecution_Internal_Implementation(Object, bExecutionSuccess);

	if(bExecutionSuccess || bUseCooldownOnFailed)
		SetCooldown(Object);
}