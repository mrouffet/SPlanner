// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_Decorator.h>
#include "SP_RandDecorator.generated.h"

/**
 *	SPlanner Random success rate decorator.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_RandDecorator : public USP_Decorator
{
	GENERATED_BODY()
	
protected:
	/**
	*	Success rate of random.
	*	Rand <= SuccessRate: Success.
	*	Rand > SuccessRate: Failure.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	float SuccessRate = 0.5f;

	bool Validate_Internal_Implementation(const USP_PlannerComponent* Planner) override;
};