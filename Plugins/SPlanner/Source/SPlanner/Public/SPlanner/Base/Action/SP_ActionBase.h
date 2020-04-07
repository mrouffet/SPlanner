// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_ActionGenerationType.h>

#include <SPlanner/Framework/SP_Object.h>
#include "SP_ActionBase.generated.h"

class USP_ActionImpl;

class USP_PlannerComponent;

/**
*	One or more ActionImpl used by Planner to generate a plan.
*/
UCLASS(Abstract, BlueprintType, EditInlineNew, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionBase : public USP_Object
{
	GENERATED_BODY()

public:
	/** The handled action step. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_ActionImpl* Impl = nullptr;

	/** The type of generation to be used by the planner. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	ESP_ActionGenerationType GenerationType = ESP_ActionGenerationType::AGT_OnceInPlan;

	/** Check Action and ActionImpl availability. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual bool IsAvailable(const USP_PlannerComponent* Planner) const;

#if WITH_EDITOR
	/**
	*	Debug string for plan logging.
	*	Impl asset name.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual FString GetDebugStr() const;
#endif
};