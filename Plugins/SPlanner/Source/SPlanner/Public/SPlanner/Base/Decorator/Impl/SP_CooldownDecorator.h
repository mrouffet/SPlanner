// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_Decorator.h>
#include "SP_CooldownDecorator.generated.h"

class USP_FloatParam;

/**
 *	SPlanner Cooldown decorator implementation.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_CooldownDecorator : public USP_Decorator
{
	GENERATED_BODY()
	
protected:
	/** The cooldown to apply. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_FloatParam* Cooldown = nullptr;

	/** Should use cooldown even if the task has failed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	bool bUseCooldownOnFailed = true;

	/**
	*	Cooldown time of all planners of this decorator.
	*	World Time since beginning + task cooldown is stored.
	*/
	TMap<const UObject*, float> Cooldowns;

	/** Set the cooldown of Planner. */
	void SetCooldown(const UObject* Object);

	bool Validate_Internal_Implementation(const UObject* Object) override;

	void PostExecution_Internal_Implementation(const UObject* Object, bool bExecutionSuccess) override;
};