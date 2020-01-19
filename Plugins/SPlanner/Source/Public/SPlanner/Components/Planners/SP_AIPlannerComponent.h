#pragma once

#include <SPlanner/Components/Planners/SP_PlannerComponent.h>
#include "SP_AIPlannerComponent.generated.h"

class USP_Task;

class USP_TargetComponent;
class USP_POIZoneComponent;

/**
*	Planner behavior.
*	Use planning to generate behavior using action set.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "Planner", ClassGroup = "SPlanner|Planner")
class SPLANNER_API USP_AIPlannerComponent : public USP_PlannerComponent
{
	GENERATED_BODY()

protected:
	/**
	*	The current index in plan executed.
	*	Avoid Plan.PopFront().
	*/
	int CurrentPlanIndex = -1;

	/** Cooldown of all tasks of this planner. */
	UPROPERTY(Transient)
	TMap<const USP_Task*, float> Cooldowns;

	/** Allocated memory for user data during task execution. */
	TArray<uint8> TaskUserData;

	/** Callback function bind to OnGoalChange. */
	UFUNCTION(Category = "SPlanner|Planner")
	void OnGoalChange_Bind(USP_PlannerComponent* Planner, USP_Goal* OldGoal, USP_Goal* NewGoal);

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

	FSP_PlannerActionSet CreatePlannerActionSet() override;

	void AskNewPlan() override;

	/** Check cooldown and call AskNewPlan after newly available task. */
	void OnPlanConstructionFailed_Implementation() override;

	/**
	*	Cancel the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Called on main thread.
	*/
	void CancelPlan_Implementation() override;

	void InitializeComponent() override;
	void UninitializeComponent() override;

	void BeginPlay() override;

public:
	/** Should reset target on goal change. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPlanner")
	bool bResetTargetOnGoalChange = true;
	
	/** Target component used. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_TargetComponent* Target = nullptr;

	/**
	*	The POI interact zone used.
	*	Used to add action set from interactible POIs.
	*/
	UPROPERTY(Transient, BlueprintReadWrite, Category = "SPlanner")
	USP_POIZoneComponent* POIZone = nullptr;

	USP_AIPlannerComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of cooldown for one behavior. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	float GetCooldown(const USP_Task* Task) const;

	/** Getter of cooldown for one behavior. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	void SetCooldown(const USP_Task* Task);

	/** Helper function for cooldown check */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	bool IsInCooldown(const USP_Task* Task) const;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};