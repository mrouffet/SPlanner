#pragma once

#include <GameFramework/Character.h>
#include "PlannerCharacter.generated.h"

class USP_PlannerComponent;

/**
*	SPlanner character implementation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|AI|Character")
class SPLANNER_API ASP_PlannerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Planning behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Planner")
	USP_PlannerComponent* Planner = nullptr;

public:
	ASP_PlannerCharacter(const FObjectInitializer& ObjectInitializer);
};