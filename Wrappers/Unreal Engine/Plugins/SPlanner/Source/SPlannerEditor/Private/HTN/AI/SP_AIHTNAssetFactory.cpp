// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <HTN/AI/SP_AIHTNAssetFactory.h>

#include <HTN/AI/SP_AIHTNAsset.h>

TSharedPtr<class FSlateStyleSet> USP_AIHTNAssetFactory::StyleSet = MakeSlyteSet("SP_AIHTNAsset", "Resources/AI/HTNAssetIcon.png", FVector2D(512, 512));

USP_AIHTNAssetFactory::USP_AIHTNAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = USP_AIHTNAsset::StaticClass();
}

UObject* USP_AIHTNAssetFactory::FactoryCreateNew(UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn)
{
	return NewObject<USP_AIHTNAsset>(InParent, InClass, InName, Flags);
}
