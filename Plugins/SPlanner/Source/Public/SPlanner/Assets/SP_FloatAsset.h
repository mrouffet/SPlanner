#pragma once

#include <Engine/DataAsset.h>
#include "SP_FloatAsset.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_FloatAssetEvent, float, Value);

/**
*	Serialized float asset.
*/
UCLASS(BlueprintType, Blueprintable, Category = "SPlanner|Asset")
class USP_FloatAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** The handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float Handle = 0.0f;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/** Callback thrown when Handle value change. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_FloatAssetEvent OnValueChange;

	/** Getter of Handle. */
	float Get() const;

	/**
	*	Setter of Handle.
	*	Trigger OnValueChange.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Asset")
	void Set(float Value);
};