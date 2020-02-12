#pragma once

#include <SPlanner/Base/Director/SP_Director.h>
#include "SP_AIDirector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_AIDirectorNumDelegate, int, AINum);

UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Director")
class SPLANNER_API ASP_AIDirector : public ASP_Director
{
	GENERATED_BODY()

protected:
	void OnRegistedPlannerActive(USP_PlannerComponent* InPlanner) override;
	void OnRegistedPlannerInactive(USP_PlannerComponent* InPlanner) override;

public:
	/** Callback event when AI get active or inactive. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Director|AI")
	FSP_AIDirectorNumDelegate OnAINumChange;
};