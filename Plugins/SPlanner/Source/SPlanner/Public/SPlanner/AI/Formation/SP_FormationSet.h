// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Engine/DataAsset.h>
#include "SP_FormationSet.generated.h"

class USP_Formation;

class USP_LODComponent;
class USP_AIPlannerComponent;

/** enum of formation focus action. */
UENUM(Category = "SPlanner|AI|Formation")
enum class ESP_FormationFocus : uint8
{
	/** Don't perform focus. */
	FLA_None			UMETA(DisplayName = "None"),

	/** Focus Lead actor. */
	FLA_Lead			UMETA(DisplayName = "Lead"),

	/** Focus Target actor. */
	FLA_Target			UMETA(DisplayName = "Target"),
};

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	FName TargetEntryName = "MainTarget";

	/** The entry name to offset vector in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	FName OffsetEntryName = "TargetOffset";

	/** The formation focus to apply. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	ESP_FormationFocus FormationFocus = ESP_FormationFocus::FLA_None;

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

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor DebugColor = FColor::Green;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	float DebugDrawTime = 2.5f;
#endif

	/** Update current formation. */
	void UpdateFormation();

	/** Set the formation focus to new Planner. */
	void SetFormationFocus(USP_AIPlannerComponent* Planner);

	/** Clear the formation focus of Planner. */
	void ClearFormationFocus(USP_AIPlannerComponent* Planner);

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

	/** Add a planner to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool Add(USP_AIPlannerComponent* Planner);

	/** Remove a planner to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Remove(USP_AIPlannerComponent* Planner);

	/**
	*	Add planners to the formation.
	*	Warning: This won't call Add().
	*	Only re-compute formation once.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool AddMultiple(const TArray<USP_AIPlannerComponent*>& InPlanners);

	/**
	*	Remove planners to the formation.
	*	Warning: This won't call Add().
	*	Only re-compute formation once.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void RemoveMultiple(const TArray<USP_AIPlannerComponent*>& InPlanners);

	/** Change formation type. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	void ChangeFormation();

	/** Reset this formation set. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Reset();
};