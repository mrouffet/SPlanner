#pragma once

#include <atomic>

#include <SPlanner/Miscs/SP_PlanState.h>
#include <SPlanner/Miscs/SP_PlanError.h>
#include <SPlanner/Miscs/Flags/SP_PlannerFlags.h>

#include <Components/ActorComponent.h>
#include "SP_PlannerComponent.generated.h"

class USP_Goal;

class USP_ActionStep;
struct FSP_PlannerActionSet;

class USP_ActionSetComponent;
class USP_PlannerLODComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_PlannerDelegate, USP_PlannerComponent*, Planner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSP_PlannerGoalDelegate, USP_PlannerComponent*, Planner, USP_Goal*, OldGoal, USP_Goal*, NewGoal);

/**
*	Planner behavior.
*	Use planning to generate behavior using action set.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, DisplayName = "Planner", ClassGroup = "SPlanner|Planner")
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

	/** Timer handle before calling ConstructPlan(). */
	FTimerHandle ConstructPlanTimer;

	/** The current executed plan. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_ActionStep*> Plan;

	/**
	*	The current plan state.
	*	Must be thread safe (called by Planner in thread).
	*	Must be init in cpp file (Standard).
	*/
	std::atomic<ESP_PlanState> PlanState;

	/** LOD component used  for plan generation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_PlannerLODComponent* LOD = nullptr;

	/**
	*	Set new plan to execute and update PlanState.
	*	Executed on external thread.
	*/
	void SetNewPlan(TArray<USP_ActionStep*>&& Plan);

	/** Create the planner action set to generate the plan with. */
	virtual FSP_PlannerActionSet CreatePlannerActionSet(float LODLevel) const;

	/**
	*	Ask a new plan generation on external thread.
	*	bInstantRequest force to ask without waiting for TimeBeforeConstructPlan (used when new goal is set).
	*/
	virtual void AskNewPlan(bool bInstantRequest = false);

	/** Construct the plan for the plannable. */
	void ConstructPlan();

	/**
	*	Construct the core plan for the plannable recursively.
	*	Return true on construction succeed.
	*	Executed on external thread.
	*/
	bool ConstructPlan_Internal(const FSP_PlannerActionSet& PlannerActions,
		TArray<USP_ActionStep*>& OutPlan,
		uint8 MaxDepth,
		uint8 CurrDepth = 0u,
		FSP_PlannerFlags PlannerFlags = FSP_PlannerFlags()) const;

	/** Callback function called when a plan's construction failed (no valid plan found). */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Planner")
	void OnPlanConstructionFailed(ESP_PlanError PlanError);

	/**
	*	Callback function to set active planner behavior.
	*	Ask new plan.
	*	Return success.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Planner")
	bool OnActive_Internal();

	/**
	*	Callback function to set inactive planner behavior.
	*	Cancel current plan and plan generation.
	*	Return success.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Planner")
	bool OnInactive_Internal();

	/**
	*	Callback function bind to LOD.OnActive.
	*	call SetEnableBehavior(true).
	*/
	UFUNCTION(Category = "SPlanner|Planner")
	void OnActiveLOD();

	/**
	*	Callback function bind to LOD.OnInactive.
	*	call SetEnableBehavior(false).
	*/
	UFUNCTION(Category = "SPlanner|Planner")
	void OnInactiveLOD();

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/**
	*	The maximum planner depth while building a plan.
	*	Plan generation complexity: O(!n), n = MaxPlannerDepth.
	*	Get overridden by SP_PlannerLODComponent.
	*/
	UPROPERTY(Config, EditAnywhere)
	uint8 DefaultMaxPlannerDepth = 5u;

	/**
	*	Minimum time to wait before constructing a new plan.
	*	Use <= 0 to compute instantly.
	*	Get overridden by SP_PlannerLODComponent.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float DefaultTimeBeforeConstructPlan = -1.0f;

	/**
	*	Whether this planner should start as active or inactive.
	*	Call OnActive / OnInactive callback in BeginPlay.
	*	Set to false for pooling use.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bStartActive = true;

	/**
	*	Whether this planner should register itself in the Director.
	*	Require Director actor in scene while true.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bAutoRegisterInDirector = true;

	/** Whether this planner should reset its goal OnActive() callback. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bResetGoalOnInactive = false;

	/** Action set component used. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_ActionSetComponent* ActionSet = nullptr;

	/**
	*	Callback event when planner become active.
	*	Called by OnActive_Internal().
	*/
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_PlannerDelegate OnActive;

	/**
	*	Callback event when planner become active.
	*	Called by OnInactive_Internal().
	*/
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_PlannerDelegate OnInactive;

	/** Callback event when goal is changed. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_PlannerGoalDelegate OnGoalChange;

	USP_PlannerComponent(const FObjectInitializer& ObjectInitializer);

	ESP_PlanState GetPlanState() const;

	/**
	*	Enable or disable behavior.
	*	Call OnActive / OnInactive.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	void SetEnableBehavior(bool bEnable);

	/**
	*	Setter of LOD.
	*	Bind OnActiveLOD / OnInactiveLOD events.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	void SetLOD(USP_PlannerLODComponent* NewLOD);

	/** Getter of Goal. */
	USP_Goal* GetGoal() const;

	/** Setter of Goal. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	void SetGoal(USP_Goal* InGoal);

	/**
	*	Cancel current executed plan.
	*	Callback function called when a plan get cancelled (by setting new goal).
	*	Return cancel succeed.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Planner")
	bool CancelPlan();
};