#pragma once

#include <SPlanner/Actions/SP_ActionMacro.h>

#include <Engine/DataAsset.h>
#include "SP_ActionStep.generated.h"

class USP_PlannerComponent;

/**
 *	Base implementation of a step of an Action.
 *	The planner generate a plan of SP_Action, composed of SP_ActionSet.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionStep : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	*	The pre-condition of the step.
	*	Must return true to be added to the plan during generation.
	*	Called on external thread.
	*/
	virtual bool PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const;

	/**
	*	The post-condition of the step.
	*	Return new planner flags to check for next pre-conditions during plan generation.
	*	Called on external thread.
	*/
	virtual uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const;
};