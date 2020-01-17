#pragma once

#include <SPlanner/Actions/SP_Action.h>
#include <SPlanner/Actions/SP_PlannerActionSet.h>

#include <Engine/DataAsset.h>
#include "SP_ActionSet.generated.h"

/**
 *	Set of Planner Action.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionSet : public UDataAsset
{
	GENERATED_BODY()

protected:
	/**
	*	List of begin actions.
	*	Only one of these will be selected.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> BeginActions;

	/** 
	*	List of core actions.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> CoreActions;

	/** 
	*	List of forced core actions.
	*	All of these will be added to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> ForcedCoreActions;

	/**
	*	List of end actions.
	*	Only one of these will be selected.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> EndActions;

public:
	/** Getter of BeginActions. */
	const TArray<FSP_Action>& GetBeginActions() const;

	/** Getter of CoreActions. */
	const TArray<FSP_Action>& GetCoreActions() const;

	/** Getter of ForcedCoreActions. */
	const TArray<FSP_Action>& GetForcedCoreActions() const;

	/** Getter of EndActions. */
	const TArray<FSP_Action>& GetEndActions() const;

	FSP_PlannerActionSet Shuffle() const;

	template <typename PredicateClass>
	FSP_PlannerActionSet Shuffle(const PredicateClass& Predicate) const
	{
		SP_RCHECK(EndActions.Num(), "No EndActions: Can't generate a valid plan!", FSP_PlannerActionSet())

		return FSP_PlannerActionSet(BeginActions, CoreActions, ForcedCoreActions, EndActions, Predicate);
	}
};