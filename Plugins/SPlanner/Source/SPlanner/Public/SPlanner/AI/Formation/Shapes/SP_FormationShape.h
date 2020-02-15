// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Params/SP_FloatParam.h>

#include <Engine/DataAsset.h>
#include "SP_FormationShape.generated.h"

class USP_Formation;

class USP_AIPlannerComponent;

/**
 *	Base formation shape.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_FormationShape : public UDataAsset
{
	GENERATED_BODY()
	
protected:
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
	USP_FormationShape(const FObjectInitializer& ObjectInitializer);

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
	virtual bool IsAvailable(const USP_Formation* Formation) const;

	/** Callback method called when Formation starts this shape. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void OnStart(const USP_Formation* Formation);

	/** Callback method called when Formation ends this shape. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void OnEnd(const USP_Formation* Formation);

	/**
	*	Compute the new position for each planners.
	*	Must be overridden in children.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|AI|Formation")
	virtual void Compute(const TArray<USP_AIPlannerComponent*>& Planners) /* = 0*/;
};