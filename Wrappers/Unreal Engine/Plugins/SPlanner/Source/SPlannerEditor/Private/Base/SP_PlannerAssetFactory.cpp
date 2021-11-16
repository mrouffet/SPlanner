// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <Base/SP_PlannerAssetFactory.h>

#include <Styling/SlateStyle.h>

USP_PlannerAssetFactory::USP_PlannerAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
}

TSharedPtr<FSlateStyleSet> USP_PlannerAssetFactory::MakeSlyteSet(const FName& ClassName, const FString& Path, const FVector2D& ImageStyle)
{
	TSharedPtr<FSlateStyleSet> StyleSet;

	const FString NameStr = ClassName.ToString();

	StyleSet = MakeShareable(new FSlateStyleSet(*(NameStr + "StyleSet")));
	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	auto brush = new FSlateImageBrush(*(FPaths::ProjectPluginsDir() + "SPlanner/" + Path), ImageStyle);

	StyleSet->Set(*(FString("ClassThumbnail.") + NameStr), brush);

	return StyleSet;
}
