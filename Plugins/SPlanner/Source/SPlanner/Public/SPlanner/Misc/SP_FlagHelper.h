// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <UObject/Object.h>
#include "SP_FlagHelper.generated.h"

/** Helper macro to check a flag in mask. */
#define SP_IS_FLAG_SET(BitMask, Flag) (BitMask & (1 << static_cast<uint8>(Flag)))

/** Helper macro to set a flag to mask. */
#define SP_SET_FLAG(BitMask, Flag) (BitMask = 1 << static_cast<uint8>(Flag))

/** Helper macro to add a flag to mask. */
#define SP_ADD_FLAG(BitMask, Flag) (BitMask |= 1 << static_cast<uint8>(Flag))

/** Helper macro to remove a flag to mask. */
#define SP_REMOVE_FLAG(BitMask, Flag) (BitMask &= ~(1 << static_cast<uint8>(Flag)))

UCLASS(Abstract, Category = "SPlanner|Misc")
class SPLANNER_API USP_FlagHelper : public UObject
{
	GENERATED_BODY()

public:
	/** Blueprint accessor of IS_FLAG_SET macro. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Misc")
	static bool IsFlagSet(int BitMask, uint8 Flag);

	/** Blueprint accessor of SET_FLAG macro. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Misc")
	static void SetFlag(int& BitMask, uint8 Flag);

	/** Blueprint accessor of ADD_FLAG macro. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Misc")
	static void AddFlag(int& BitMask, uint8 Flag);

	/** Blueprint accessor of REMOVE_FLAG macro. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Misc")
	static void RemoveFlag(int& BitMask, uint8 Flag);
};