// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_ActionMacro.h>

#include <SPlanner/Framework/SP_DataAsset.h>
#include "SP_ActionStep.generated.h"

class USP_Decorator;

class USP_PlanGenInfos;
class USP_PlannerComponent;

/**
 *	Base implementation of a step of an Action.
 *	The planner generate a plan of SP_Action, composed of SP_ActionSet.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionStep : public USP_DataAsset
{
	GENERATED_BODY()
	
protected:
	/** Additionnal condition to validate. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Decorator*> Decorators;

public:
	/** Check action availability for Planner. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Action|Task")
	virtual bool IsAvailable(const USP_PlannerComponent* Planner) const;

	/** Call PostExecution of Decorators. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Action")
	virtual void PostExecution(const USP_PlannerComponent* Planner, bool bExecutionSuccess);

	/**
	*	The pre-condition of the step.
	*	Must return true to be added to the plan during generation.
	*	Called on external thread.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action")
	bool PreCondition(const USP_PlanGenInfos* Infos) const;

	/**
	*	The post-condition of the step.
	*	Must return true to be added to the plan during generation.
	*	Called on external thread.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action")
	bool PostCondition(USP_PlanGenInfos* Infos) const;

	/**
	*	Reset the previous added post-conditions.
	*	Return value is used for parent debug check only.
	*	Called on external thread.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action")
	bool ResetPostCondition(USP_PlanGenInfos* Infos) const;
};