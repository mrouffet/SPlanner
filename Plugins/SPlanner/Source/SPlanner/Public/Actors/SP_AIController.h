#pragma once

#include <AIController.h>
#include "SP_AIController.generated.h"

class USP_PlannerComponent;

/**
*	SPlanner controller implementation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Controller")
class SPLANNER_API ASP_AIController : public AAIController
{
	GENERATED_BODY()

protected:
	/** Planning behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_PlannerComponent* Planner = nullptr;

	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

public:
	ASP_AIController(const FObjectInitializer& ObjectInitializer);
};