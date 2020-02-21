// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Planner/SP_AIPlannerNotify.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>
#include "SP_AIPlannerComponent.generated.h"

class USP_Task;

class USP_POIZoneComponent;

class APawn;
class ASP_AIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIPlannerNotifyDelegate, USP_AIPlannerComponent*, Planner, ESP_AIPlannerNotify, Notify);

/**
*	Planner behavior.
*	Use planning to generate behavior using action set.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "SP_AIPlanner", ClassGroup = "SPlanner|Planner")
class SPLANNER_API USP_AIPlannerComponent : public USP_PlannerComponent
{
	GENERATED_BODY()

protected:
	/**
	*	The current index in plan executed.
	*	Avoid Plan.PopFront().
	*/
	int CurrentPlanIndex = -1;

	/**
	*	Cooldown time of all tasks of this planner.
	*	World Time since beginning + task cooldown is stored.
	*/
	TMap<const USP_Task*, float> Cooldowns;

	/** Allocated memory for user data during task execution. */
	TArray<uint8> TaskUserData;

	/**
	*	Begin the next task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Return Task.Begin().
	*/
	bool BeginNextTask();

	/**
	*	Execute the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Called on main thread.
	*/
	void ExecuteTask(float DeltaTime);

	/**
	*	End the task of the Plan.
	*	return Task.End().
	*/
	bool EndTask(USP_Task* Task);

	FSP_PlannerActionSet CreatePlannerActionSet(float LODLevel, bool* bCanBeAchievedPtr = nullptr) const override;

	void AskNewPlan(bool bInstantRequest = false) override;

	/**
	*	Construction plan method.
	*	Use linear plan construction algorithm.
	*	Return true on construction succeed.
	*/
	bool ConstructPlan_Internal(FSP_PlannerActionSet& PlannerActions, TArray<USP_ActionStep*>& OutPlan, uint8 MaxDepth, float LODLevel) const override;

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
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|AI")
	FSP_AIPlannerNotifyDelegate OnNotify;

	/**
	*	The POI interact zone used.
	*	Used to add action set from interactible POIs.
	*/
	UPROPERTY(Transient, BlueprintReadWrite, Category = "SPlanner|AI")
	USP_POIZoneComponent* POIZone = nullptr;

	USP_AIPlannerComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of the previous executed ActionStep in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_ActionStep* GetPrevActionStep() const;

	/** Getter of previous executed ActionSteps in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	TArray<USP_ActionStep*> GetPrevActionSteps() const;

	/** Getter of current executed ActionStep. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_ActionStep* GetCurrentActionStep() const;

	/** Getter of the next ActionStep to execute in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	USP_ActionStep* GetNextActionStep() const;

	/** Getter of next ActionSteps to execute in Plan*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner|AI")
	TArray<USP_ActionStep*> GetNextActionSteps() const;

	/** Getter of Task user data. */
	uint8* GetTaskUserData();

	/** const Getter of Task user data. */
	const uint8* GetTaskUserData() const;

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