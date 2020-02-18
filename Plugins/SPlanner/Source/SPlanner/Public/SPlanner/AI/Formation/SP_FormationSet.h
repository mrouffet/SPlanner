// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Engine/DataAsset.h>
#include "SP_FormationSet.generated.h"

class USP_Formation;

class USP_LODComponent;
class USP_AIPlannerComponent;

/**
 *	Base implementation of AI Formation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_FormationSet : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** The leader actor of the formation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	AActor* LeadActor = nullptr;

	/** The cached LOD component of LeadActor. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_LODComponent* LeadLOD = nullptr;

	/** The entry name to set main target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FName TargetEntryName = "MainTarget";

	/** The entry name to offset vector in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FName OffsetEntryName = "TargetOffset";

	/** The saved current update time. */
	float CurrTickTime = 0.0f;

	/** The saved LeadLocation to compare formation threshold. */
	FVector SavedLeadLocation = FVector::ZeroVector;

	/**
	*	The rate of changing formation when a new planner is joining.
	*	Set < 0.0f to never random.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float RandomChangeFormationRate = 0.05f;

	/**	Whether the same formation can be choose again when calling ChangeFormation(). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bCanSelectSameFormationWhenChange = false;

	/** All possible formations from this set. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Formation*> Formations;

	/** List of all planners on this formation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_AIPlannerComponent*> Planners;

	/** Current formation in use. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
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
	void SetFormationFocus(USP_AIPlannerComponent* Planner);

	/** Clear the formation focus of Planner. */
	void ClearFormationFocus(USP_AIPlannerComponent* Planner);

	/** Check if every planner in InPlanners are contained or not in Planners. */
	bool CheckAreContained(const TArray<USP_AIPlannerComponent*>& InPlanners, bool bShouldBeContained);

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
	/**
	*	The targeted actor.
	*	Used for formation rotation.
	*/
	UPROPERTY(Transient, BlueprintReadWrite, Category = "SPlanner")
	AActor* TargetActor = nullptr;

	/** Getter of LeadActor. */
	AActor* GetLeadActor() const;

	/** Getter of LeadLOD. */
	USP_LODComponent* GetLeadLOD() const;

	/**
	*	Set the LeadActor.
	*	Must be done once at start before doing anything else.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	void SetLeadActor(AActor* NewLeadActor);

	/** Add planners to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool Add(const TArray<USP_AIPlannerComponent*>& InPlanners);

	/** Remove planners to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool Remove(const TArray<USP_AIPlannerComponent*>& InPlanners);

	/** Add a planner to the formation. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool AddSingle(USP_AIPlannerComponent* Planner);

	/** Remove a planner to the formation. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool RemoveSingle(USP_AIPlannerComponent* Planner);

	/** Change formation type. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool ChangeFormation();

	/** Update this formation set. */
	void Tick(float DeltaSeconds);

	/** Reset this formation set. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Reset();
};