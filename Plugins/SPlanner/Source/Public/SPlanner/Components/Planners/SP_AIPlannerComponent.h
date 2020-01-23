#pragma once

#include <SPlanner/Components/Planners/SP_PlannerComponent.h>
#include "SP_AIPlannerComponent.generated.h"

class USP_AITask;

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
	TMap<const USP_AITask*, float> Cooldowns;

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
	*	Return Task.Begin().
	*/
	bool BeginNextTask();

	/**
	*	Execute the current task of the Plan.
	*	Use CurrentPlanIndex and Plan.
	*	Called on main thread.
	*/
	void ExecuteTask(float DeltaTime);

	FSP_PlannerActionSet CreatePlannerActionSet(float LODLevel) const override;

	void AskNewPlan(bool bInstantRequest = false) override;

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

	void InitializeComponent() override;
	void UninitializeComponent() override;

	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	/** Getter of the previous executed ActionStep in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	USP_ActionStep* GetPrevActionStep() const;

	/** Getter of previous executed ActionSteps in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	TArray<USP_ActionStep*> GetPrevActionSteps() const;

	/** Getter of current executed ActionStep. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	USP_ActionStep* GetCurrentActionStep() const;

	/** Getter of the next ActionStep to execute in Plan. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	USP_ActionStep* GetNextActionStep() const;

	/** Getter of next ActionSteps to execute in Plan*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	TArray<USP_ActionStep*> GetNextActionSteps() const;


	/** Getter of cooldown for one behavior. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	float GetCooldown(const USP_AITask* Task) const;

	/** Getter of cooldown for one behavior. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	void SetCooldown(const USP_AITask* Task);

	/** Helper function for cooldown check */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Planner")
	bool IsInCooldown(const USP_AITask* Task) const;
};