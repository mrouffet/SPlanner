#pragma once

#include <AIController.h>
#include "SP_AIController.generated.h"

class USP_AIPlannerComponent;

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
	USP_AIPlannerComponent* Planner = nullptr;

	/** Callback function called when plan get cancelled. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Controller")
	void OnPlanCancel(USP_PlannerComponent* InPlanner);

	void PostInitializeComponents() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

public:
	/** Name of the PlannerComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName PlannerComponentName;

	ASP_AIController(const FObjectInitializer& ObjectInitializer);

	/** Enable or disable Planner behavior. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Controller")
	void SetEnableBehavior(bool bEnable);
};