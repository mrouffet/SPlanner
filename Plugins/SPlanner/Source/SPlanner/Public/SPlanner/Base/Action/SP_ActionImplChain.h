// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_ActionImpl.h>
#include "SP_ActionImplChain.generated.h"

/**
 *	Chain of action step.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionImplChain : public USP_ActionImpl
{
	GENERATED_BODY()

protected:
	/** The handled action steps. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Action|Chain")
	TArray<USP_ActionImpl*> Impls;

	bool IsAvailable(const USP_PlannerComponent* Planner) const override;

	/** The pre-condition of the chain (ie: chain of pre-condition / post-condition through Impls). */
	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;

	/** The post-condition of the action (ie: chain of all post-condition of Impls). */
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
};