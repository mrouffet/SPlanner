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

	/**
	*	The rate of changing formation when a new planner is joining.
	*	Set < 0.0f to never random.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float RandomChangeFormationRate = 0.05f;

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

	/** Update current formation. */
	void UpdateFormation();

	/** Find all suitable formations. */
	virtual TArray<USP_Formation*> FindAvailableFormations() const;

	/** Select a random formation from AvailableFormations using weights. */
	USP_Formation* SelectRandomFormation(const TArray<USP_Formation*>& AvailableFormations) const;

	/** Predicate to add a formation to AvailableFormations list during generation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool PredicateAvailable(USP_Formation* Formation) const;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
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

	/** Add a planner to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool Add(USP_AIPlannerComponent* Planner);

	/** Remove a planner to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Remove(USP_AIPlannerComponent* Planner);

	/** Change formation type. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	void ChangeFormation();

	/** Reset this formation set. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Reset();
};