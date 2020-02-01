// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <UObject/Object.h>
#include "SP_BlueprintHelper.generated.h"

UCLASS(Abstract, Category = "SPlanner|Misc")
class SPLANNER_API USP_BlueprintHelper : public UObject
{
	GENERATED_BODY()

public:
	/** Blueprint accessor of any loaded object. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Misc", meta = (Keywords="GetObject"))
	static UObject* GetObject(UObject* Object);
};