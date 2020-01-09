#pragma once

#include <Tasks/SP_Action.h>

#include <Engine/DataAsset.h>
#include "SP_ActionSet.generated.h"

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