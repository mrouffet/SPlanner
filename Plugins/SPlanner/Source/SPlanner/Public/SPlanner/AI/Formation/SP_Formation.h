// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Params/SP_FloatParam.h>

#include <SPlanner/AI/Formation/SP_FormationConstructionType.h>

#include <Engine/DataAsset.h>
#include "SP_Formation.generated.h"

class USP_Formation;

class USP_AIPlannerComponent;

/**
 *	Base formation shape.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_Formation : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	/** Method of construction. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	ESP_FormationConstructionType ConstructionType = ESP_FormationConstructionType::FCT_Dichotomy;

	/** Minimum of AI for this shape. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	int MinNum = 1;

	/** Maximum of AI for this shape. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	int MaxNum = 4;

	/** Whether cooldown should be shared across all formations. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bShareCooldown = false;

	/**
	*	Saved world time with cooldown.
	*	Require bShareCooldown == true.
	*/
	float SavedTimeCooldown = -1.0f;

	/**
	*	Weight of this shape.
	*	Increase weight for chances to be selected.
	*	LOD Level will be computed from formation's lead actor.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FSP_FloatParam Weight;

	/**
	*	Cooldown of this shape.
	*	Time before this shape become available again.
	*	LOD Level will be computed from formation's lead actor.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FSP_FloatParam Cooldown;

public:
	USP_Formation(const FObjectInitializer& ObjectInitializer);

	/** Getter of MinNum. */
	int GetMinNum() const;

	/** Getter of MaxNum. */
	int GetMaxNum() const;

	/** Getter of Weight. */
	float GetWeight(float LODLevel = -1.0f) const;

	/** Getter of Cooldown. */
	float GetCooldown(float LODLevel = -1.0f) const;

	/** Getter of bShareCooldown. */
	bool IsCooldownShared() const;

	UFUNCTION(BlueprintPure, Category = "SPlanner|AI|Formation")
	virtual bool IsAvailable(const USP_FormationSet* FormationSet) const;

	/** Callback method called when Formation starts this shape. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void OnStart(const USP_FormationSet* FormationSet);

	/** Callback method called when Formation ends this shape. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void OnEnd(const USP_FormationSet* FormationSet);

	/**
	*	Compute the offset position for each planners.
	*	Must be overridden in children.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	virtual bool Compute(AActor* LeadActor, AActor* TargetActor, const TArray<USP_AIPlannerComponent*>& Planners, TArray<FVector>& Offsets) /* = 0*/;

	/** Reset this formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Reset();
};