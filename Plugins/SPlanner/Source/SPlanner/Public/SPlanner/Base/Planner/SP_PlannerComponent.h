// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <atomic>

#include <SPlanner/Base/Planner/SP_PlanState.h>
#include <SPlanner/Base/Planner/SP_PlanError.h>

#include <Components/ActorComponent.h>
#include "SP_PlannerComponent.generated.h"

class USP_Goal;

class USP_ActionStep;
struct FSP_PlannerActionSet;

class USP_BlackboardComponent;
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

protected:
	/**
	*	The current targeted goal.
	*	Editor set default value.
	*/
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_Goal* Goal = nullptr;
	
	/**
	*	The cached next goal.
	*	Goal will be changed during Tick.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPlanner")
	USP_Goal* NextGoal = nullptr;


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
	*	Update Goal using NextGoal.
	*	Update PlanState depending on generation.
	*/
	void UpdateGoal();

	/**
	*	Set new plan to execute and update PlanState.
	*	Executed on external thread.
	*/
	void SetNewPlan(TArray<USP_ActionStep*>&& Plan);

	/** Create the planner action set to generate the plan with. */
	virtual FSP_PlannerActionSet CreatePlannerActionSet(float LODLevel, bool* bCanBeAchievedPtr = nullptr) const;

	/**
	*	Ask a new plan generation on external thread.
	*	bInstantRequest force to ask without waiting for TimeBeforeConstructPlan (used when new goal is set).
	*/
	virtual void AskNewPlan(bool bInstantRequest = false);

	/** Construct the plan for the plannable. */
	void ConstructPlan();

	/**
	*	Construction plan method.
	*	Must be overridden in children.
	*	Return true on construction succeed.
	*/
	virtual bool ConstructPlan_Internal(FSP_PlannerActionSet& PlannerActions, TArray<USP_ActionStep*>& OutPlan, uint8 MaxDepth, float LODLevel) const;


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
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/**
	*	The maximum planner depth while building a plan.
	*	Plan generation complexity: O(!n), n = MaxPlannerDepth.
	*	Get overridden by SP_PlannerLODComponent.
	*/
	UPROPERTY(Config, EditAnywhere)
	uint8 DefaultMaxPlannerDepth = 7u;

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

	/** Blackboard component used. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_BlackboardComponent* Blackboard = nullptr;

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

	/** Callback event when a new plan is asked. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_PlannerDelegate OnAskPlan;

	/** Callback event when plan get cancelled. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_PlannerDelegate OnPlanCancel;

	/** Callback event when goal is changed. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_PlannerGoalDelegate OnGoalChange;


	USP_PlannerComponent(const FObjectInitializer& ObjectInitializer);

	/** Get casted Blackboard used by this planner. */
	template <typename T>
	T* GetBlackboard() const
	{
		return Cast<T>(Blackboard);
	}

	ESP_PlanState GetPlanState() const;

	/** Getter of enable/disable behavior. */
	UFUNCTION(BlueprintPure, Category = "SPlanner")
	bool IsBehaviorEnabled() const;

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

	/**
	*	Setter of Goal.
	*	Goal get cached and will be update in next Tick().
	*	use bForce to force goal set (ie no goal transition check).
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	void SetGoal(USP_Goal* InGoal, bool bForce = false);

	/** Getter of Plan. */
	const TArray<USP_ActionStep*>& GetPlan() const;

	/**
	*	Cancel current executed plan.
	*	Callback function called when a plan get cancelled (by setting new goal).
	*	Return cancel succeed.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	virtual bool CancelPlan();

#if WITH_EDITOR
	bool IsSelectedInEditor() const override;
#endif
};