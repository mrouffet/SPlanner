// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPlannerDemoGameMode.h"
#include "SPlannerDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASPlannerDemoGameMode::ASPlannerDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
