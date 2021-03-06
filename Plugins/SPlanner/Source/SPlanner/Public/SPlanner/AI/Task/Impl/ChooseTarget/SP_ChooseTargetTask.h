// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskImpl.h>
#include "SP_ChooseTargetTask.generated.h"

/**
 *	Base implementation of ChooseTarget task.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_ChooseTargetTask : public USP_TaskImpl
{
	GENERATED_BODY()
	
protected:
	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FName OutTargetEntryName = "None";

	/** Can target again even if a previous target has been set in plan. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bAllowReTarget = false;

	/** Whether the target should be visible. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bTargetVisible = true;

	/**
	*	Offset to apply to the center of the field of view in Pawn's local space.
	*	Use GetFOVCenter() accessor.
	*/
	UPROPERTY(EditAnywhere, Category = "SPlanner|Task|Target")
	FVector LocalOffset = FVector::ZeroVector;

	/**
	*	Extent of minimum (near) field of view in Pawn's local space.
	*	Set < 0.0f for unlimited bound.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector MinLocalExtent = FVector(-1.0f, -1.0f, -1.0f);

	/**
	*	Extent of maximum (far) field of view in Pawn's local space.
	*	Set < 0.0f for unlimited bound.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector MaxLocalExtent = FVector(-1.0f, -1.0f, -1.0f);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor MinFOVDebugColor = FColor::Orange;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor MaxFOVDebugColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor LineDebugColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	float DebugDrawTime = 2.0f;
#endif

	bool IsInBox_Internal(const APawn* Pawn, const FVector& BoxLocalExtent, const FVector& Location) const;

	/** Check whether Min box's extent are valid. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target")
	bool HasValidMinBox() const;

	/** Check whether Max box's extent are valid. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target")
	bool HasValidMaxBox() const;

	/** Check whether Location is in Pawn's Min Box. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target")
	bool IsInMinBox(const APawn* Pawn, const FVector& InLocation) const;

	/** Check whether Location is in Pawn's Max Box. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target")
	bool IsInMaxBox(const APawn* Pawn, const FVector& InLocation) const;

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

#if WITH_EDITOR
	void DrawDebug(const USP_AIPlannerComponent* Planner, const FVector& TargetLocation);
#endif

public:
	/** Get the World FOV center using Pawn and LocalOffset. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Task|Target")
	FVector GetFOVCenter(const APawn* Pawn) const;

	/** Get the World FOV min extent using MinLocalExtent. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Task|Target")
	FVector GetFOVMinExtent(const APawn* Pawn) const;

	/** Get the World FOV min extent using MaxLocalExtent. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Task|Target")
	FVector GetFOVMaxExtent(const APawn* Pawn) const;
};