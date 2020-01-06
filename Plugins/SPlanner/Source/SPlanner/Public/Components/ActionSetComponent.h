#pragma once

#include <Components/ActorComponent.h>
#include "ActionSetComponent.generated.h"

class USP_Goal;
class USP_ActionSet;

/**
*	Action set component used by PlannerComponent.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "ActionSet", ClassGroup = "SPlanner|ActionSet")
class SPLANNER_API USP_ActionSetComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Action sets depending on current goal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Planner")
	TMap<USP_Goal*, USP_ActionSet*> ActionsSets;

public:
	USP_ActionSetComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "SPlanner|Planner")
	USP_ActionSet* GetActionSet(USP_Goal* Goal);
};