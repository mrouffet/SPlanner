#pragma once

#include <SPlanner/Base/Actions/SP_ActionStep.h>
#include "SP_ActionStepChain.generated.h"

/**
 *	Chain of action step.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionStepChain : public USP_ActionStep
{
	GENERATED_BODY()

protected:
	/** The handled action steps. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Chain")
	TArray<USP_ActionStep*> Steps;

public:
	/** The pre-condition of the chain (ie: chain of pre-condition / post-condition through Steps). */
	bool PreCondition(const USP_PlannerComponent* Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;

	/** The post-condition of the action (ie: chain of all post-condition of Steps). */
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;
};