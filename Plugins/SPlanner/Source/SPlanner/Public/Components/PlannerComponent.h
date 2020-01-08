#pragma once

#include <atomic>

#include <Miscs/PlanState.h>
#include <Miscs/Flags/PlannerFlags.h>

#include <Tasks/Action.h>

#include <Components/ActorComponent.h>
#include "PlannerComponent.generated.h"

class USP_Goal;
class USP_Task;

class USP_TargetComponent;
class USP_ActionSetComponent;
class USP_InteractZoneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSP_PlannerGoalDelegate, USP_PlannerComponent*, Planner, USP_Goal*, OldGoal, USP_Goal*, NewGoal);

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
	/**
	*	The current targeted goal.
	*	Editor set default value.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_Goal* Goal = nullptr;

	/** Timer handle before calling ConstructPlan() */
	FTimerHandle ConstructPlanTimer;

	/** Cooldown of all tasks of this planner. */
	UPROPERTY(Transient)
	TMap<const USP_Task*, float> Cooldowns;

	/** The current executed plan. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
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
	*	Must be init in cpp file (Standard).
	*/
	std::atomic<ESP_PlanState> PlanState;

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
	bool ConstructPlan_Internal(const TArray<FSP_Action>& AvailableActions, TArray<USP_Task*>& Plan, FSP_PlannerFlags PlannerFlags = FSP_PlannerFlags(), uint8 CurrDepth = 0) const;

	/** Check new task availability to request new plan generation. */
	void CheckCooldowns();

	/**
	*	Begin the next task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Return Result == ESP_PlanExecutionState::PES_Succeed.
	*/
	bool BeginNextTask();

	/**
	*	Execute the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Called on main thread.
	*/
	void ExecuteTask(float DeltaTime);

	/**
	*	End the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Return Result == ESP_PlanExecutionState::PES_Succeed.
	*/
	bool EndTask();

	/**
	*	Cancel the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Called on main thread.
	*/
	void CancelTask();


	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/**
	*	Whether this planner should register itself in the AIDirector.
	*	Require AIDirector actor in scene while true.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bAutoRegisterInAIDirector = false;

	/**
	*	Minimum time to wait before constructing a new plan.
	*	Use <= 0 to compute instantly.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float TimeBeforeConstructPlan = -1.0f;

	/** Action set component used. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_ActionSetComponent* ActionSet = nullptr;

	/** Target component used. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_TargetComponent* Target = nullptr;
	
	/**
	*	The POI interact zone used.
	*	Used to add action set from interactible POIs.
	*/
	UPROPERTY(Transient, BlueprintReadWrite, Category = "SPlanner")
	USP_InteractZoneComponent* InteractZone = nullptr;

	/** Callback event when goal is changed. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_PlannerGoalDelegate OnGoalChange;

	USP_PlannerComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of Goal. */
	USP_Goal* GetGoal() const;

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

	/**
	*	Get a shuffled move list based on actions' weight and random.
	*	Executed on external thread.
	*	Return true on success (ie: valid action set for current goal).
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	bool GetShuffledActions(TArray<FSP_Action>& ShuffledActions) const;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};