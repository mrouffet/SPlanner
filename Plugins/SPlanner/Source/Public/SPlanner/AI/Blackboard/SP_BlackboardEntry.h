#pragma once

#include "SP_BlackboardEntry.generated.h"

class USP_BlackboardKey;

USTRUCT(BlueprintType, Blueprintable, Category = "SPlanner|Blackboard")
struct SPLANNER_API FSP_BlackboardEntry
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
	USP_BlackboardKey* Key = nullptr;
};