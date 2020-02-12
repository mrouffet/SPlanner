#pragma once

#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey.h>
#include "SP_BlackboardKey_Int.generated.h"

/**
*	Blackboard key base implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Int", ClassGroup = "SPlanner|Blackboard|Key")
class USP_BlackboardKey_Int : public USP_BlackboardKey
{
	GENERATED_BODY()

protected:
	/** Handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Handle;

public:
	/** Getter of Handle. */
	int GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|Key")
	void SetValue(int Value);
};