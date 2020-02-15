// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_AIBlackboardEntry.generated.h"

class USP_AIBlackboardKey;

USTRUCT(BlueprintType, Blueprintable, Category = "SPlanner|AI|Blackboard")
struct SPLANNER_API FSP_AIBlackboardEntry
{
	GENERATED_BODY()
	
	/** Key name. Only one key can have a name in the same SP_BlackboardAsset. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FName Name = "None";

#if WITH_EDITORONLY_DATA
	/** Editor only description. */
	UPROPERTY(EditAnywhere, Category = "SPlanner", meta=(ToolTip="Optional description (Editor only)."))
	FString EntryDescription;
#endif

	/** The class of the handled object. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner")
	USP_AIBlackboardKey* Key = nullptr;
};