// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Framework/SP_DataAsset.h>
#include "SP_FormationSet.generated.h"

class USP_Formation;

class USP_AIPlannerComponent;

/**
 *	Base implementation of AI Formation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_FormationSet : public USP_DataAsset
{
	GENERATED_BODY()

protected:
	/** The leader actor of the formation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|AI")
	AActor* LeadActor = nullptr;

	/** The cached AI Planner component of LeadActor (if lead has one). */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|AI")
	USP_AIPlannerComponent* LeadPlanner = nullptr;

	/**
	*	The targeted actor.
	*	Used for formation rotation.
	*/
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|AI")
	AActor* TargetActor = nullptr;

	/** The entry name to set the output position target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AI")
	FName OutputTargetEntryName = "MainTarget";

	/**
	*	The entry name to set the target actor in Blackboard.
	*	Set to "None" to disable.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AI")
	FName TargetActorEntryName = "None";

	/** The saved current update time. */
	float CurrTickTime = 0.0f;

	/** The saved LeadLocation to compare formation threshold. */
	FVector SavedLeadLocation = FVector::ZeroVector;

	/**
	*	The rate of changing formation when a new planner is joining.
	*	Set < 0.0f to never random.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AI")
	float RandomChangeFormationRate = 0.05f;

	/**	Whether the same formation can be choose again when calling ChangeFormation(). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AI")
	bool bCanSelectSameFormationWhenChange = false;

	/** All possible formations for this set. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AI")
	TArray<USP_Formation*> Formations;

	/** List of all planners on this formation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|AI")
	TArray<USP_AIPlannerComponent*> Planners;

	/** Current formation in use. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|AI")
	USP_Formation* CurrentFormation = nullptr;

	/** Cooldown of all formations from this set. */
	TMap<const USP_Formation*, float> Cooldowns;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor DebugColor = FColor::Green;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor DebugTargetColor = FColor::Purple;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	float DebugDrawTime = 1.0f;
#endif

	/** Set the formation focus to new Planner. */
	void SetFormationFocus(USP_AIPlannerComponent* Planner) const;

	/** Clear the formation focus of Planner. */
	void ClearFormationFocus(USP_AIPlannerComponent* Planner) const;

	/** Check if every planner in InPlanners are contained or not in Planners. */
	bool CheckAreContained(const TArray<const USP_AIPlannerComponent*>& InPlanners, bool bShouldBeContained) const;

	/** Init the data for InPlanners. */
	virtual void InitPlannersData(const TArray<USP_AIPlannerComponent*>& InPlanners);

	/** UnInit the data for InPlanners. */
	virtual void UnInitPlannersData(const TArray<USP_AIPlannerComponent*>& InPlanners);

	/** Find all suitable formations. */
	virtual TArray<USP_Formation*> FindAvailableFormations(int PlannerNum) const;

	/** Select a random formation from AvailableFormations using weights. */
	USP_Formation* SelectRandomFormation(const TArray<USP_Formation*>& AvailableFormations) const;

	/**
	*	Try to change formation for new PlannerNum.
	*	return ChaneFormation result.
	*/
	bool TryChangeFormation(int PlannerNum);

	/** Internal Implementation of change formation. */
	bool ChangeFormation_Internal(const TArray<USP_Formation*>& AvailableFormations);

	void UpdateFormation();

	/** Predicate to add a formation to AvailableFormations list during generation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool PredicateAvailable(USP_Formation* Formation) const;

	/** Apply new computed offsets to Planners. */
	void ApplyOffsets(const TArray<FVector>& Offsets);

#if WITH_EDITOR
	/** Check if at least one object of the formation is selected. */
	bool IsSelected() const;

	/** Draw Formation debug. */
	void DrawDebug() const;

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/** Getter of LeadActor. */
	AActor* GetLeadActor() const;

	/** Getter of TargetActor. */
	AActor* GetTargetActor() const;

	/** Setter of TargetActor. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	void SetTargetActor(AActor* NewTargetActor);

	/**
	*	Set the LeadActor.
	*	Must be done once at start before doing anything else.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	void SetLeadActor(AActor* NewLeadActor);


	/** Whether Planner can be added to the formation. */
	bool CanAdd(const USP_AIPlannerComponent* Planner) const;

	/** Whether InPlanners can be added to the formation (enough places and not already in this formation). */
	bool CanAdd(const TArray<const USP_AIPlannerComponent*>& InPlanners) const;

	/** Whether Planner can be removed to the formation. */
	bool CanRemove(const USP_AIPlannerComponent* Planner) const;

	/** Whether InPlanners can be removed from the formation (still enough planner and contained in this formation). */
	bool CanRemove(const TArray<const USP_AIPlannerComponent*>& InPlanners) const;

	
	/** Add one planner to the formation. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool Add(USP_AIPlannerComponent* Planner);

	/** Add planners to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool AddRange(const TArray<USP_AIPlannerComponent*>& InPlanners);

	/** Remove one planner to the formation. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool Remove(USP_AIPlannerComponent* Planner);

	/** Remove planners to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool RemoveRange(const TArray<USP_AIPlannerComponent*>& InPlanners);


	/** Change formation type. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool ChangeFormation();

	/** Update this formation set. */
	void Tick(float DeltaSeconds);

	/** Reset this formation set. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Reset();
};