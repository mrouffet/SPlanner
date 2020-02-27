// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Planner/SP_AIPlannerNotify.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>
#include "SP_AIPlannerComponent.generated.h"

class USP_Task;

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
	/**
	*	The current index in plan executed.
	*	Avoid Plan.PopFront().
	*/
	int CurrentPlanIndex = 0;

	/**
	*	Cooldown time of all tasks of this planner.
	*	World Time since beginning + task cooldown is stored.
	*/
	TMap<const USP_Task*, float> Cooldowns;

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

	FSP_PlannerActionSet CreatePlannerActionSet(float LODLevel, bool* bCanBeAchievedPtr = nullptr) const override;

	void AskNewPlan(bool bInstantRequest = false) override;

	/**
	*	Construction plan method.
	*	Use linear plan construction algorithm.
	*	Return true on construction succeed.
	*/
	bool ConstructPlan_Internal(FSP_PlannerActionSet& PlannerActions, TArray<USP_ActionStep*>& OutPlan, USP_PlanGenInfos* PlanGenInfos, uint8 MaxDepth, float LODLevel) const override;

	/** Check cooldown and call AskNewPlan after newly available task. */
	void OnPlanConstructionFailed_Implementation(ESP_PlanError PlanError) override;

	/**
	*	Cancel the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Called on main thread.
	*/
	bool CancelPlan() override;

	bool OnActive_Internal_Implementation() override;
	bool OnInactive_Internal_Implementation() override;

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

	/** Getter of the previous executed Task in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_Task* GetPrevTask() const;

	/** Getter of previous executed Tasks in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	TArray<USP_Task*> GetPrevTasks() const;

	/** Getter of current executed Task. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_Task* GetCurrentTask() const;

	/** Getter of the next Task to execute in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_Task* GetNextTask() const;

	/** Getter of next Tasks to execute in Plan*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	TArray<USP_Task*> GetNextTasks() const;

	/** Getter of cooldown for one task. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	float GetCooldown(const USP_Task* Task) const;

	/** Getter of cooldown for one task. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner|AI")
	void SetCooldown(const USP_Task* Task);

	/** Helper function for cooldown check */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	bool IsInCooldown(const USP_Task* Task) const;

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

#if WITH_EDITOR
	bool IsSelectedInEditor() const override;
#endif
};