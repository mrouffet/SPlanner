// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Math/Vector.h>
#include <Containers/Array.h>

class AActor;

class USP_AIPlannerComponent;


/** Infos provided by FormationSet. */
struct FSP_FormationSetInfos
{
	/** List of planners. */
	const TArray<USP_AIPlannerComponent*>& Planners;

	/**
	*	List of computed offset for each planners.
	*	Will be modified by formation implementation.
	*/
	TArray<FVector>& Offsets;

	/** The lead actor of the formation. */
	const AActor* LeadActor = nullptr;

	/** The target actor of the formation. */
	const AActor* TargetActor = nullptr;
};


/** Info for one planner in formation. */
struct FSP_PlannerFormatioInfos
{
	/** The handled planner. */
	USP_AIPlannerComponent* Planner = nullptr;

	/** The angle between Direction and Pawn to Lead direction. */
	float Angle = 0.0f;

	/**
	*	The reference to the offset vector to set.
	*	Use reference because PlannerInfos will be sorted and won't match the original Planners array order.
	*/
	FVector& Offset;

	bool operator<(const FSP_PlannerFormatioInfos& Rhs) const;
};


/** Infos provided by the formation. */
struct FSP_FormationInfos
{
	/** List of planners in formation. */
	TArray<FSP_PlannerFormatioInfos> PlannerInfos;

	/** The reference direction of the formation. */
	FVector BaseDirection;

	/** The lead actor of the formation. */
	const AActor* LeadActor = nullptr;

	/** The target actor of the formation. */
	const AActor* TargetActor = nullptr;

	FSP_FormationInfos(const FSP_FormationSetInfos& SetInfos, const FVector& Direction = FVector::ZeroVector);
};