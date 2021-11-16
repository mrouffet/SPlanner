// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <Base/AI/SP_AIPlannerAssetFactory.h>

#include "SP_AIGOAPAssetFactory.generated.h"

UCLASS(ClassGroup = "SPlanner|GOAP|AI|Editor")
class SPLANNEREDITOR_API USP_AIGOAPAssetFactory : public USP_AIPlannerAssetFactory
{
	GENERATED_BODY()

public:
	USP_AIGOAPAssetFactory(const FObjectInitializer& ObjectInitializer);

	UObject* FactoryCreateNew(UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn) override;
};
