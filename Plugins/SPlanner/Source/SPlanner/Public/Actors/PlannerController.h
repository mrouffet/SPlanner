#pragma once

#include <AIController.h>
#include "PlannerController.generated.h"

class USP_PlannerComponent;

/**
*	SPlanner controller implementation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Controller")
class SPLANNER_API ASP_PlannerController : public AAIController
{
	GENERATED_BODY()

protected:
	/** Planning behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_PlannerComponent* Planner = nullptr;

	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

public:
	ASP_PlannerController(const FObjectInitializer& ObjectInitializer);
};