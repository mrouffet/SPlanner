// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <GameFramework/Actor.h>
#include "SP_Director.generated.h"

class USP_Goal;

class USP_PlannerComponent;
class USP_ActionSetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_DirectorGoalRegisterDelegate, USP_Goal*, InGoal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_DirectorPlannerRegisterDelegate, USP_PlannerComponent*, InPlanner);

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

	/** Registered active planners. */
	TArray<USP_PlannerComponent*> ActivePlanners;

	/** Registered inactive planners. */
	TArray<USP_PlannerComponent*> InactivePlanners;

	/** Registered active goals. */
	TArray<USP_Goal*> Goals;

	/**
	*	Callback function when planner get registered.
	*	Called by static RegisterPlanner().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Director")
	void RegisterPlanner_Internal(USP_PlannerComponent* InPlanner);

	/**
	*	Callback function when planner get unregistered.
	*	Called by static UnRegisterPlanner().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Director")
	void UnRegisterPlanner_Internal(USP_PlannerComponent* InPlanner);

	/**
	*	Callback function when goal get registered.
	*	Called by static RegisterGoal().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Director")
	void RegisterGoal_Internal(USP_Goal* InGoal);

	/**
	*	Callback function when goal get unregistered.
	*	Called by static UnRegisterGoal().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Director")
	void UnRegisterGoal_Internal(USP_Goal* InGoal);

	/** Callback function bound to registered Planner.OnActive. */
	UFUNCTION(Category = "SPlanner|Director")
	virtual void OnRegistedPlannerActive(USP_PlannerComponent* InPlanner);

	/** Callback function bound to registered Planner.OnInactive. */
	UFUNCTION(Category = "SPlanner|Director")
	virtual void OnRegistedPlannerInactive(USP_PlannerComponent* InPlanner);

	void PreInitializeComponents() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** Callback event when a planner get registered. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_DirectorPlannerRegisterDelegate OnRegisterPlanner;

	/** Callback event when a planner get unregistered. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_DirectorPlannerRegisterDelegate OnUnRegisterPlanner;

	/** Callback event when a goal get registered. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_DirectorGoalRegisterDelegate OnRegisterGoal;

	/** Callback event when a goal get unregistered. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_DirectorGoalRegisterDelegate OnUnRegisterGoal;

	ASP_Director(const FObjectInitializer& ObjectInitializer);

	/** Getter of total planner registered num. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Director")
	int GetPlannerNum(bool bIncludeInactive = false) const;

	/** Getter of ActivePlanners. */
	const TArray<USP_PlannerComponent*>& GetActivePlanners() const;

	/** Getter of InactivePlanners. */
	const TArray<USP_PlannerComponent*>& GetInactivePlanners() const;

	/** Getter of active and inactive planners. */
	TArray<USP_PlannerComponent*> GetAllPlanners() const;

	/** Getter of activee planners with goal. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	const TArray<USP_PlannerComponent*>& GetPlannersWithGoal(USP_Goal* Goal);

	/** Getter of Instance. */
	UFUNCTION(BlueprintPure, DisplayName = "Get SP_Director", Category = "SPlanner|Director", meta=(Keywords = "SP Director"))
	static ASP_Director* GetInstance();

	/**
	*	Register planner in Director instance planner list.
	*	Require Director actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static void RegisterPlanner(USP_PlannerComponent* InPlanner);

	/**
	*	Unregister planner from Director instance planner list.
	*	Require Director actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static void UnRegisterPlanner(USP_PlannerComponent* InPlanner);

	/**
	*	Register goal in Director instance.
	*	Require Director actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static void RegisterGoal(USP_Goal* InGoal);

	/**
	*	Unregister goal from Director instance.
	*	Require Director actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static void UnRegisterGoal(USP_Goal* InGoal);

	/**
	*	Set goal to all selected planner in editor.
	*	In editor, only selected planner will be set (or all planner if no selected and bApplyToAllIfNoSelected).
	*	In non SP_DEBUG build, every registered planner will be set.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	void SetAllSelectedPlannerGoal(USP_Goal* NewGoal, bool bApplyToAllIfNoSelected = true);
};