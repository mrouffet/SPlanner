#pragma once

#include <SPlanner/Actions/AITasks/Impl/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetPOITask.generated.h"

class USP_POIComponent;

/**
 *	Choose target implementation with POI spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|AITask|Target")
class SPLANNER_API USP_ChooseTargetPOITask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
protected:
	/** Should target nearest POI or pick a random one. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AITask|Target|POI")
	bool bTargetNearest = true;

	/**
	*	List of allowed POI actor type to target.
	*	Set empty to allow every types.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AITask|Target|POI")
	TArray<TSubclassOf<AActor>> AllowedPOIActorTypes;

	/**
	*	List of ignored POI actor type to target.
	*	Set empty to allow every types.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AITask|Target|POI")
	TArray<TSubclassOf<AActor>> IgnoredPOIActorTypes;

	/**
	*	Predicate to validate to choose a POI.
	*	Can be overridden in children, otherwise return true.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action|AITask|Target")
	bool Predicate(const USP_AIPlannerComponent* Planner, const USP_POIComponent* TargetPOI) const;

public:
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};