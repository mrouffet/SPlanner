// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <GameFramework/Character.h>
#include "SP_Character.generated.h"

class USP_AIBlackboardComponent;
class USP_POIZoneComponent;
class USP_AILODComponent;

/**
*	SPlanner character implementation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Character")
class SPLANNER_API ASP_Character : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Blackboard used by Planner. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_AIBlackboardComponent* Blackboard = nullptr;

	/** POI Interact zone behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_POIZoneComponent* POIZone = nullptr;

	/** LOD behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_AILODComponent* LOD = nullptr;

public:
	ASP_Character(const FObjectInitializer& ObjectInitializer);
};