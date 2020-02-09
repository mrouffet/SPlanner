#pragma once

#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey.h>
#include "SP_BlackboardKey_Float.generated.h"

/**
*	Blackboard key float implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Float", ClassGroup = "SPlanner|Blackboard|Key")
class USP_BlackboardKey_Float : public USP_BlackboardKey
{
	GENERATED_BODY()

protected:
	/** Handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Handle = 0.0f;

public:
	/** Getter of Handle. */
	float GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|Key")
	void SetValue(float Value);
};