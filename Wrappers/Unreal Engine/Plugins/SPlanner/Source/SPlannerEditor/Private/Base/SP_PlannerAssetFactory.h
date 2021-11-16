// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <Factories/Factory.h>

#include "SP_PlannerAssetFactory.generated.h"

UCLASS(Abstract, ClassGroup = "SPlanner|Editor")
class SPLANNEREDITOR_API USP_PlannerAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	USP_PlannerAssetFactory(const FObjectInitializer& ObjectInitializer);
};
