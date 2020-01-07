#pragma once

#include <Tasks/Action.h>

#include <Engine/DataAsset.h>
#include "ActionSet.generated.h"

/**
 *	Set of Planner Action.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionSet : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** List of available actions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> Actions;

public:
	/** Getter of Actions. */
	const TArray<FSP_Action>& GetActions() const;
};