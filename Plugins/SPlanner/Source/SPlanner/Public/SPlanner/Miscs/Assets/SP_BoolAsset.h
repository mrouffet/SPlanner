// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Assets/SP_Asset.h>
#include "SP_BoolAsset.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_BoolAssetEvent, bool, Value);

/**
*	Serialized float asset.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Asset")
class SPLANNER_API USP_BoolAsset : public USP_Asset
{
	GENERATED_BODY()

protected:
	/** The handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool Handle = false;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/** Callback thrown when Handle value change. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_BoolAssetEvent OnValueChange;

	/** Getter of Handle. */
	bool Get() const;

	/**
	*	Setter of Handle.
	*	Trigger OnValueChange.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Asset")
	void Set(bool Value);
};