// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_ActionStep.h>
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

	/** The pre-condition of the chain (ie: chain of pre-condition / post-condition through Steps). */
	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;

	/** The post-condition of the action (ie: chain of all post-condition of Steps). */
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
};