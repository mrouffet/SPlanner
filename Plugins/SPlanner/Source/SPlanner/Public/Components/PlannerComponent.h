#pragma once

#include <atomic>

#include <Miscs/PlanState.h>
#include <Tasks/Action.h>

#include <Components/ActorComponent.h>
#include "PlannerComponent.generated.h"

class USP_Goal;
class USP_Task;
class USP_ActionSet;
class USP_POIActionSet;

class USP_TargetComponent;

/**
*	Planner behavior.
*	Use planning to generate behavior using action set.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "Planner", ClassGroup = "SPlanner|Planner")
class SPLANNER_API USP_PlannerComponent : public UActorComponent
{
	GENERATED_BODY()

	// Allow async task to call ConstructPlan().
	friend class FSP_PlanConstructTask;

protected:
	/** Action sets depending on current goal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Planner")
	TMap<USP_Goal*, USP_ActionSet*> ActionsSets;

	/** The action from Point of interest (ie: added to possible moves). */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|Planner")
	TArray<USP_POIActionSet*> POIActionSets;

	/**
	*	The current targeted goal.
	*	Editor set default value.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Planner")
	USP_Goal* Goal = nullptr;

	/** Cooldown of all tasks of this planner. */
	UPROPERTY(Transient)
	TMap<const USP_Task*, float> Cooldowns;

	/** The current executed plan. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|Planner")
	TArray<USP_Task*> Plan;

	/**
	*	The current index in plan.
	*	Avoid Plan.PopFront().
	*/
	int CurrentPlanIndex = -1;

	/** Allocated memory for user data during task execution. */
	TArray<uint8> TaskUserData;

	/**
	*	The current plan state.
	*	Must be thread safe (called by Planner in thread).
	*	Must be init in cpp file (PS4 Standard).
	*/
	std::atomic<ESP_PlanState> PlanState;

	/** Cached target component. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|Planner")
	USP_TargetComponent* Target = nullptr;

	/**
	*	Set new plan to execute and update PlanState.
	*	Executed on external thread.
	*/
	void SetNewPlan(TArray<USP_Task*>&& Plan);

	/**	Ask a new plan generation on external thread. */
	void AskNewPlan();

	/** Construct the plan for the plannable. */
	void ConstructPlan();

	/**
	*	Construct the plan for the plannable recursively.
	*	Return true on construction succeed.
	*	Executed on external thread.
	*/
	bool ConstructPlan_Internal(const TArray<FSP_Action>& AvailableActions, TArray<USP_Task*>& Plan, int PlannerFlags = 0, uint8 CurrDepth = 0) const;

	/** Check new task availability to request new plan generation. */
	void CheckCooldowns();

	/**
	*	Begin the next task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Return Result == ESP_PlanExecutionState::PES_Succeed.
	*/
	bool BeginNextTask();

	/**
	*	End the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Return Result == ESP_PlanExecutionState::PES_Succeed.
	*/
	bool EndTask();

	/**
	*	Execute the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Called on main thread.
	*/
	void ExecuteTask(float DeltaTime);


	void BeginPlay() override;

public:
	USP_PlannerComponent(const FObjectInitializer& ObjectInitializer);

	/** Setter of Goal. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	void SetGoal(USP_Goal* InGoal);

	/** Getter of cooldown for one behavior. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	float GetCooldown(const USP_Task* Task) const;

	/** Getter of cooldown for one behavior. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	void SetCooldown(const USP_Task* Task);

	/** Helper function for cooldown check */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	bool IsInCooldown(const USP_Task* Task) const;

	/** Getter of Target. */
	USP_TargetComponent* GetTarget() const;

	/**
	*	Get a shuffled move list based on actions' weight and random.
	*	Executed on external thread.
	*	Return true on success (ie: valid action set for current goal).
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	bool GetShuffledActions(TArray<FSP_Action>& ShuffledActions) const;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};