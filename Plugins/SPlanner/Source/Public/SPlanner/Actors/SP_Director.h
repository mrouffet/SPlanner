#pragma once

#include <GameFramework/Actor.h>
#include "SP_Director.generated.h"

class USP_Goal;

class USP_PlannerComponent;
class USP_ActionSetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_AIDirectorRegisterDelegate, USP_PlannerComponent*, InPlanner);

UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Director")
class SPLANNER_API ASP_Director : public AActor
{
	GENERATED_BODY()

protected:
	/**
	*	Singleton instance of Director.
	*	Director actor must be put in scene.
	*/
	static ASP_Director* Instance;

	/** All planner registered (currently in use) for each goal. */
	TMap<USP_Goal*, TArray<USP_PlannerComponent*>> GoalPlannersMap;


	/**
	*	Callback function when planner get registered.
	*	Called by static Register().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Director")
	void OnRegister_Internal(USP_PlannerComponent* InPlanner);

	/**
	*	Callback function when planner get unregistered.
	*	Called by static UnRegister().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Director")
	void OnUnRegister_Internal(USP_PlannerComponent* InPlanner);

	/** Callback function bound to registered Planner.OnGoalChange. */
	UFUNCTION(Category = "SPlanner|Director")
	virtual void OnRegistedPlannerGoalChange(USP_PlannerComponent* InPlanner, USP_Goal* OldGoal, USP_Goal* NewGoal);

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** Callback event while planner get registered. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_AIDirectorRegisterDelegate OnRegister;

	/** Callback event while planner get unregistered. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_AIDirectorRegisterDelegate OnUnRegister;

	ASP_Director(const FObjectInitializer& ObjectInitializer);

	/** Getter of total planner registered num. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Director")
	int GetPlannerNum() const;

	/** Getter (query) of all planners registered. */
	TArray<USP_PlannerComponent*> QueryAllPlanners() const;

	/**
	*	Blueprint getter (query) of all planners registered.
	*	Need to be BlueprintCallable and non-const to force Unreal to cache resulted array.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	TArray<USP_PlannerComponent*> QueryAllPlanners();

	/** Getter (query) of planners with goal in GoalPlannersMap. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	const TArray<USP_PlannerComponent*>& QueryAllPlannersWithGoal(USP_Goal* Goal);

	/** Getter of Instance. */
	UFUNCTION(BlueprintPure, DisplayName = "Get SP_Director", Category = "SPlanner|Director")
	static ASP_Director* GetInstance();

	/**
	*	Register planner in Director instance planner list.
	*	Require Director actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static void Register(USP_PlannerComponent* InPlanner);

	/**
	*	Unregister planner from Director instance planner list.
	*	Require Director actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static void UnRegister(USP_PlannerComponent* InPlanner);

	/**
	*	Try to register planner in Director instance planner list.
	*	Does nothing without Director actor in scene.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static bool TryRegister(USP_PlannerComponent* InPlanner);

	/**
	*	Try to unregister planner from Director instance planner list.
	*	Does nothing without Director actor in scene.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static bool TryUnRegister(USP_PlannerComponent* InPlanner);
};