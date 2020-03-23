// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/Impl/SP_CooldownDecorator.h>

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

void USP_CooldownDecorator::SetCooldown(const USP_PlannerComponent* Planner)
{
	float PlannerCoolDown = Cooldown->Query(Planner);

	// Never save no cooldown.
	if (PlannerCoolDown <= 0.0f)
		return;

	// Add 0.001f to ensure float precision.
	Cooldowns.FindOrAdd(Planner) = Planner->GetWorld()->GetTimeSeconds() + PlannerCoolDown + 0.001f;
}

bool USP_CooldownDecorator::Validate_Internal_Implementation(const USP_PlannerComponent* Planner)
{
	SP_DECORATOR_SUPER_VALIDATE(Planner)

	const float* const CooldownPtr = Cooldowns.Find(Planner);

	float PlannerCoolDown = CooldownPtr ? *CooldownPtr - Planner->GetWorld()->GetTimeSeconds() : -1.0f;

	return PlannerCoolDown > 0.0f;
}

void USP_CooldownDecorator::OnValidationSuccess_Implementation(const USP_PlannerComponent* Planner)
{
	Super::OnValidationSuccess_Implementation(Planner);

	SetCooldown(Planner);
}
void USP_CooldownDecorator::OnValidationFailure_Implementation(const USP_PlannerComponent* Planner)
{
	Super::OnValidationFailure_Implementation(Planner);

	if (bUseCooldownOnFailed)
		SetCooldown(Planner);
}