// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Framework/SP_DataAsset.h>
#include "SP_ActionSet.generated.h"

class USP_Action;

struct FSP_PlannerActionSet;

class USP_PlannerComponent;

/**
 *	Set of Planner Action.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionSet : public USP_DataAsset
{
	GENERATED_BODY()

protected:
	/** The class to use for Weight. */
	TSubclassOf<USP_Action> ActionClass = nullptr;

	/**
	*	List of begin actions.
	*	Only one of these will be selected.
	*/
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Action*> BeginActions;

	/** 
	*	List of core actions.
	*/
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Action*> CoreActions;

	/** 
	*	List of forced actions.
	*	All of these will be added to the plan.
	*/
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Action*> ForcedActions;

	/**
	*	List of end actions.
	*	Only one of these will be selected.
	*/
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Action*> EndActions;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** Find a Null Action in array and instantiate a new one using ActionClass. */
	void ForceActionClass(TArray<USP_Action*>& Actions);
#endif

public:
	USP_ActionSet(const FObjectInitializer& ObjectInitializer);


	/** Getter of BeginActions. */
	const TArray<USP_Action*>& GetBeginActions() const;

	/** Getter of CoreActions. */
	const TArray<USP_Action*>& GetCoreActions() const;

	/** Getter of ForcedCoreActions. */
	const TArray<USP_Action*>& GetForcedActions() const;

	/** Getter of EndActions. */
	const TArray<USP_Action*>& GetEndActions() const;

	/**
	*	Create PlannerActionSet from action set.
	*	Shuffle every actions using random and weights.
	*	Call FSP_PlannerActionSet::Make().
	*/
	FSP_PlannerActionSet Shuffle(const USP_PlannerComponent* Planner, bool* bCanBeAchievedPtr = nullptr) const;
};