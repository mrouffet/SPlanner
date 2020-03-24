// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_PlannerDecorator.h>
#include "SP_RequireStepDecorator.generated.h"

class USP_ActionStep;

/**
 *	Check required steps of the currently generated plan.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_RequireStepDecorator : public USP_PlannerDecorator
{
	GENERATED_BODY()
	
protected:
	/** Required steps that must be already added to the plan. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	TArray<USP_ActionStep*> RequieredSteps;

	/** Whether the required steps must use the specific order. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	bool bUseRequiredStepOrder = true;

	bool PreCondition_Validate_Internal_Implementation(const USP_PlanGenInfos* Infos) override;

public:
	USP_RequireStepDecorator(const FObjectInitializer& ObjectInitializer);
};