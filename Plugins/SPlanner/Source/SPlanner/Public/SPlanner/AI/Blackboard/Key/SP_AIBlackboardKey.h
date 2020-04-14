// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <UObject/Object.h>
#include "SP_AIBlackboardKey.generated.h"

class USP_AIBlackboardComponent;

/**
*	Blackboard key base implementation.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard|AI|Key")
class USP_AIBlackboardKey : public UObject
{
	GENERATED_BODY()

protected:
	/** Whether this key should be reset on plan failed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bShouldResetOnPlanFailed = false;

	/** Whether this key should be reset on plan cancelled. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bShouldResetOnPlanCancelled = false;

public:
	/** Getter of bShouldResetOnPlanFailed */
	bool ShouldResetOnPlanFailed() const;

	/** Getter of bShouldResetOnPlanCancelled */
	bool ShouldResetOnPlanCancelled() const;

	/**
	*	reset the value from the original key.
	*	Must be overridden in children.
	*/
	virtual void ResetValue(const USP_AIBlackboardKey* OriginalKey) /* = 0 */;

	/**
	*	Create a new instance of this key.
	*	Must be overridden in children.
	*/
	virtual USP_AIBlackboardKey* CreateInstance(USP_AIBlackboardComponent* Outer) /* = 0 */;
};