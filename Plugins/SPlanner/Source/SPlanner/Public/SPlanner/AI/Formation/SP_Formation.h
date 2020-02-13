// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Engine/DataAsset.h>
#include "SP_Formation.generated.h"

class USP_FormationShape;

class USP_LODComponent;
class USP_AIPlannerComponent;

/**
 *	Base implementation of AI Formation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_Formation : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** The leader actor of the formation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	AActor* LeadActor = nullptr;

	/** The cached LOD component of LeadActor. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_LODComponent* LeadLOD = nullptr;

	/** All possible shapes for this formation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_FormationShape*> Shapes;

	/** List of all planners on this formation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_AIPlannerComponent*> Planners;

	/** Current Shape in use. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_FormationShape* CurrentShape = nullptr;

	/** Cooldown of all shapes of this formation. */
	TMap<const USP_FormationShape*, float> Cooldowns;

	/** Update current formation. */
	void UpdateFormation();

	/** Find all suitable shapes. */
	virtual TArray<USP_FormationShape*> FindAvailableShapes() const;

	/** Select a random shape from AvailableShapes using weights. */
	USP_FormationShape* SelectRandomShapes(const TArray<USP_FormationShape*>& AvailableShapes) const;

	/** Predicate to add Shape to AvailableShape list during generation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool PredicateAvailable(USP_FormationShape* Shape) const;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/** Getter of LeadActor. */
	AActor* GetLeadActor() const;

	/** Getter of LeadLOD. */
	USP_LODComponent* GetLeadLOD() const;

	/**
	*	Initialize the LeadActor.
	*	Must be done once at start before doing anything else.
	*/
	void InitLeadActor(AActor* NewLeadActor);

	/** Add a planner to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	bool Add(USP_AIPlannerComponent* Planner);

	/** Remove a planner to the formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Remove(USP_AIPlannerComponent* Planner);
};