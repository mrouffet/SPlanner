// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Params/SP_FloatParam.h>

#include <SPlanner/AI/Formation/SP_FormationInfos.h>
#include <SPlanner/AI/Formation/SP_FormationConstructionType.h>

#include <Engine/DataAsset.h>
#include "SP_Formation.generated.h"

class USP_FormationSet;

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

	/** Should use the lead actor's forward as reference dir. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Circle")
	bool bUseLeadForwardAsReference = false;

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

	/** Compute the base direction. */
	virtual FVector ComputeBaseDirection(const FSP_FormationSetInfos& SetInfos);

	/** Compute the signed angle between BaseDirection and Planner to LeadActor direction. */
	float ComputeSignedAngle(const APawn* Pawn, const AActor* LeadActor, const FVector& BaseDirection);
	float ComputeSignedAngle(const FVector& PawnLocation, const FVector& LeadLocation, const FVector& BaseDirection);

	/**
	*	Construct formation by dichotomy.
	*	Must be overridden in children.
	*/
	virtual void ConstructDichotomy(FSP_FormationInfos& Infos);

	/**
	*	Construct formation point by point.
	*	Must be overridden in children.
	*/
	virtual void ConstructPointByPoint(FSP_FormationInfos& Infos);

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

	/** Compute the formation from set infos. */
	void Compute(FSP_FormationSetInfos SetInfos);

	/** Reset this formation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|AI|Formation")
	void Reset();
};