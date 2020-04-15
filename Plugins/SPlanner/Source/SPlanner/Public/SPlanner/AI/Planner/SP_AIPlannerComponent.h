// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Planner/SP_AIPlannerNotify.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>
#include "SP_AIPlannerComponent.generated.h"

class USP_TaskImpl;

class USP_AILODComponent;
class USP_POIZoneComponent;

class APawn;
class ASP_AIController;

class USP_TaskInfos;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIPlannerNotifyDelegate, USP_AIPlannerComponent*, Planner, ESP_AIPlannerNotify, Notify);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSP_AIPlannerTaskNotifyDelegate, USP_AIPlannerComponent*, Planner, ESP_AIPlannerNotify, Notify, USP_TaskInfos*, InTaskInfos);

/**
*	Planner behavior.
*	Use planning to generate behavior using action set.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "SP_AIPlanner", ClassGroup = "SPlanner|Planner|AI")
class SPLANNER_API USP_AIPlannerComponent : public USP_PlannerComponent
{
	GENERATED_BODY()

protected:
	/** LOD component used  for plan generation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_AILODComponent* LOD = nullptr;

	/** Timer used by Freeze(). */
	FTimerHandle FrozenTimer;

	/**
	*	The current index in plan executed.
	*	Avoid Plan.PopFront().
	*/
	int CurrentPlanIndex = 0;

	/**
	*	Current executed task infos.
	*	Must be UPROPERTY() to avoid garbage collection.
	*/
	UPROPERTY()
	USP_TaskInfos* TaskInfos;

	/**
	*	Execute the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Called on main thread.
	*/
	void ExecuteTask(float DeltaTime);

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

	FSP_PlannerActionSet CreatePlannerActionSet(bool* bCanBeAchievedPtr = nullptr) const override;

	void AskNewPlan(bool bInstantRequest = false) override;

	/**
	*	Construction plan method.
	*	Use linear plan construction algorithm.
	*	Return true on construction succeed.
	*/
	bool ConstructPlan_Internal(USP_PlanGenInfos* Infos) const override;

	/** Check cooldown and call AskNewPlan after newly available task. */
	void OnPlanConstructionFailed_Implementation(ESP_PlanError PlanError) override;

	/**
	*	Cancel the current Plan.
	*	Called on main thread.
	*/
	bool CancelPlan_Implementation() override;


	bool OnActive_Internal_Implementation() override;
	bool OnInactive_Internal_Implementation() override;

	float QueryTimeBeforeConstructPlan_Implementation() const override;
	int QueryMaxPlannerDepth_Implementation() const override;

	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** Callback thrown when planner receive a Notify(). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|AI")
	FSP_AIPlannerNotifyDelegate OnNotify;

	/** Callback thrown when planner receive a Notify(). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|AI")
	FSP_AIPlannerTaskNotifyDelegate OnNotifyTask;

	/**
	*	The POI interact zone used.
	*	Used to add action set from interactible POIs.
	*/
	UPROPERTY(Transient, BlueprintReadWrite, Category = "SPlanner|AI")
	USP_POIZoneComponent* POIZone = nullptr;

	USP_AIPlannerComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of LOD level. */
	UFUNCTION(BlueprintPure, Category = "SPlanner")
	float GetLODLevel() const;

	/**
	*	Setter of LOD.
	*	Bind OnActiveLOD / OnInactiveLOD events.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	void SetLOD(USP_AILODComponent* NewLOD);

	/** Getter of the previous executed Task in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_TaskImpl* GetPrevTask() const;

	/** Getter of previous executed Tasks in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	TArray<USP_TaskImpl*> GetPrevTasks() const;

	/** Getter of current executed Task. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_TaskImpl* GetCurrentTask() const;

	/** Getter of the next Task to execute in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_TaskImpl* GetNextTask() const;

	/** Getter of next Tasks to execute in Plan*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	TArray<USP_TaskImpl*> GetNextTasks() const;

	/** Notify by calling OnNotify(). */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner|AI")
	void Notify(ESP_AIPlannerNotify Notify);

	/** Get Controller this planner is attached to. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	ASP_AIController* GetController() const;

	/** Get casted Controller this planner is attached to. */
	template <typename T>
	T* GetController() const
	{
		return Cast<T>(GetController());
	}

	/** Get pawn controlled by this planner. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	APawn* GetPawn() const;

	/** Get casted pawn controlled by this planner. */
	template <typename T>
	T* GetPawn() const
	{
		return Cast<T>(GetPawn());
	}

	
	/** Callback function called when the plan execution failed. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Planner")
	void OnPlanFailed();

	/**
	*	Freeze the planner behavior for Time.
	*	Freeze behavior do not cancel the current plan.
	*	Plan will continue on UnFreeze().
	*	Use Time < 0.0f for infinite time (wait for UnFreeze call).
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Planner")
	void Freeze(float Time = -1.0f);

	/**
	*	Unfreeze the planner behavior.
	*	Resume the previous computed plan.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Planner")
	void UnFreeze();

	/** Whether this planner is currently frozen. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	bool IsFrozen();

#if WITH_EDITOR
	bool IsSelectedInEditor() const override;
#endif
};