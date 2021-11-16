// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/SP_AIGOAPAssetFactory.h>

#include <GOAP/AI/SP_AIGOAPAsset.h>

TSharedPtr<class FSlateStyleSet> USP_AIGOAPAssetFactory::StyleSet = MakeSlyteSet("SP_AIGOAPAsset", "Resources/AI/GOAPAssetIcon.png", FVector2D(512, 512));

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
