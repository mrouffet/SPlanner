#pragma once

#include <SPlanner/Base/Actions/SP_ActionGenerationType.h>

#include "SP_ActionBase.generated.h"

class USP_ActionStep;

/**
*	One or more ActionStep used by Planner to generate a plan.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct SPLANNER_API FSP_ActionBase
{
	GENERATED_BODY()

	/** The handled action step. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_ActionStep* Step = nullptr;

	/** The type of generation to be used by the planner. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	ESP_ActionGenerationType GenerationType = ESP_ActionGenerationType::AGT_OnceInPlan;

	FSP_ActionBase() = default;
	FSP_ActionBase(USP_ActionStep* InStep, ESP_ActionGenerationType InGenerationType = ESP_ActionGenerationType::AGT_OnceInPlan);

#if WITH_EDITOR
	/**
	*	Debug string for plan logging.
	*	Step asset name.
	*/
	FString GetDebugStr() const;
#endif
};