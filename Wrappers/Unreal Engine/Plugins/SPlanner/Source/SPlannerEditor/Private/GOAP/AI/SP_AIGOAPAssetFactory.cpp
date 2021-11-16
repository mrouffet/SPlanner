// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/SP_AIGOAPAssetFactory.h>

#include <GOAP/AI/SP_AIGOAPAsset.h>

USP_AIGOAPAssetFactory::USP_AIGOAPAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = USP_AIGOAPAsset::StaticClass();
}

UObject* USP_AIGOAPAssetFactory::FactoryCreateNew(UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn)
{
	return NewObject<USP_AIGOAPAsset>(InParent, InClass, InName, Flags);
}
