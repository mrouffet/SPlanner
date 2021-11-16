// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <Factories/Factory.h>

#include "SP_PlannerAssetFactory.generated.h"

class FSlateStyleSet;

UCLASS(Abstract, ClassGroup = "SPlanner|Editor")
class SPLANNEREDITOR_API USP_PlannerAssetFactory : public UFactory
{
	GENERATED_BODY()

protected:
	static TSharedPtr<FSlateStyleSet> MakeSlyteSet(const FName& ClassName, const FString& Path, const FVector2D& ImageStyle);

public:
	USP_PlannerAssetFactory(const FObjectInitializer& ObjectInitializer);
};
