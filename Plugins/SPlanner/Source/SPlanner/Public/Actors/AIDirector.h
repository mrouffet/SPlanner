#pragma once

#include <GameFramework/Actor.h>
#include "AIDirector.generated.h"

class USP_Goal;

class USP_PlannerComponent;
class USP_ActionSetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_AIDirectorRegisterDelegate, USP_PlannerComponent*, InPlanner);

UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Director")
class SPLANNER_API ASP_AIDirector : public AActor
{
	GENERATED_BODY()

protected:
	/**
	*	Singleton instance of AIDirector.
	*	AIDirector actor must be put in scene.
	*/
	static ASP_AIDirector* Instance;

	/** All planner registered (currently in use) for each goal. */
	TMap<USP_Goal*, TArray<USP_PlannerComponent*>> GoalPlannersMap;

	/** Use planning behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_PlannerComponent* Planner = nullptr;

	/** Action set used by Planner. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_ActionSetComponent* ActionSet = nullptr;

	/**
	*	Callback function when planner get registered.
	*	Called by static Register().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|AIDirector")
	void OnRegister_Internal(USP_PlannerComponent* InPlanner);

	/**
	*	Callback function when planner get unregistered.
	*	Called by static UnRegister().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|AIDirector")
	void OnUnRegister_Internal(USP_PlannerComponent* InPlanner);

	/** Callback function bound to registered Planner.OnGoalChange. */
	UFUNCTION(Category = "SPlanner|AIDirector")
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

	ASP_AIDirector(const FObjectInitializer& ObjectInitializer);

	/** Getter of total planner registered num. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|AIDirector")
	int GetPlannerNum() const;

	/** Getter of planners with goal in GoalPlannersMap. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|AIDirector")
	const TArray<USP_PlannerComponent*>& GetAllPlannersWithGoal(USP_Goal* Goal);

	/**
	*	Register planner in AIDirector instance planner list.
	*	Require AIDirector actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AIDirector")
	static void Register(USP_PlannerComponent* InPlanner);

	/**
	*	Unregister planner from AIDirector instance planner list.
	*	Require AIDirector actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AIDirector")
	static void UnRegister(USP_PlannerComponent* InPlanner);

	/**
	*	Try to register planner in AIDirector instance planner list.
	*	Does nothing without AIDirector actor in scene.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AIDirector")
	static bool TryRegister(USP_PlannerComponent* InPlanner);

	/**
	*	Try to unregister planner from AIDirector instance planner list.
	*	Does nothing without AIDirector actor in scene.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AIDirector")
	static bool TryUnRegister(USP_PlannerComponent* InPlanner);
};