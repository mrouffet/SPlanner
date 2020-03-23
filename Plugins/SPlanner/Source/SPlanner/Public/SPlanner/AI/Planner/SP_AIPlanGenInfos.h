// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/SP_FlagHelper.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenFlags.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>
#include "SP_AIPlanGenInfos.generated.h"

/**
*	AI specialization of structure infos for plan generation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Planner|AI")
class SPLANNER_API USP_AIPlanGenInfos : public USP_PlanGenInfos
{
	GENERATED_BODY()

protected:
	/** Array of main flags generated in plan. */
	TArray<ESP_AIPlanGenFlags> Flags;

	/** Array of flags for each blackboard entries generated in plan. */
	TMap<FName, TArray<ESP_AIBBPlanGenFlags>> BlackboardFlags;


#pragma region Base
	/** Internal implementation of multiple AddFlags using recursion, specialization for one argument. */
	template <typename T>
	void AddFlags_Internal(T Flag)
	{
		Flags.Add(Flag);
	}

	/** Internal implementation of multiple AddFlags using recursion, specialization for variadic arguments. */
	template <typename FirstArg, typename... OtherArgs>
	void AddFlags_Internal(FirstArg FirstFlag, OtherArgs... OtherFlags)
	{
		AddFlags_Internal(FirstFlag);
		AddFlags_Internal(OtherFlags...);
	}


	/** Internal implementation of multiple RemoveFlags using recursion, specialization for one argument. */
	template <typename T>
	void RemoveFlags_Internal(T Flag)
	{
		Flags.Remove(Flag);
	}

	/** Internal implementation of multiple RemoveFlags using recursion, specialization for variadic arguments. */
	template <typename FirstArg, typename... OtherArgs>
	void RemoveFlags_Internal(FirstArg FirstFlag, OtherArgs... OtherFlags)
	{
		RemoveFlags_Internal(FirstFlag);
		RemoveFlags_Internal(OtherFlags...);
	}
#pragma endregion


#pragma region Blackboard
	/** Internal implementation of multiple AddBlackboardFlags using recursion, specialization for one argument. */
	template <typename T>
	static void AddBlackboardFlags_Internal(TArray<ESP_AIBBPlanGenFlags>& Flags, T Flag)
	{
		Flags.Add(Flag);
	}

	/** Internal implementation of multiple AddBlackboardFlags using recursion, specialization for variadic arguments. */
	template <typename FirstArg, typename... OtherArgs>
	static void AddBlackboardFlags_Internal(TArray<ESP_AIBBPlanGenFlags>& Flags, FirstArg FirstFlag, OtherArgs... OtherFlags)
	{
		AddBlackboardFlags_Internal(Flags, FirstFlag);
		AddBlackboardFlags_Internal(Flags, OtherFlags...);
	}


	/** Internal implementation of multiple RemoveBlackboardFlags using recursion, specialization for one argument. */
	template <typename T>
	static void RemoveBlackboardFlags_Internal(TArray<ESP_AIBBPlanGenFlags>& Flags, T Flag)
	{
		Flags.Remove(Flag);
	}

	/** Internal implementation of multiple RemoveBlackboardFlags using recursion, specialization for variadic arguments. */
	template <typename FirstArg, typename... OtherArgs>
	static void RemoveBlackboardFlags_Internal(TArray<ESP_AIBBPlanGenFlags>& Flags, FirstArg FirstFlag, OtherArgs... OtherFlags)
	{
		RemoveBlackboardFlags_Internal(Flags, FirstFlag);
		RemoveBlackboardFlags_Internal(Flags, OtherFlags...);
	}
#pragma endregion

public:
#pragma region Base
	/** Whether Flag is set in Flags. */
	bool IsFlagSet(ESP_AIPlanGenFlags Flag) const;


	/** Add one flag. */
	void AddFlag(ESP_AIPlanGenFlags Flag);

	/** Add multiple flags. */
	template <typename... Args>
	void AddFlags(Args... OtherFlags)
	{
		AddFlags_Internal(OtherFlags...);
	}


	/** Remove one flag. */
	void RemoveFlag(ESP_AIPlanGenFlags Flag);

	/** Add multiple flags. */
	template <typename... Args>
	void RemoveFlags(Args... OtherFlags)
	{
		RemoveFlags_Internal(OtherFlags...);
	}
#pragma endregion


#pragma region Blackboard
	/** Whether Flag is set in EntryName's Flags. */
	bool IsBlackboardFlagSet(const FName& EntryName, ESP_AIBBPlanGenFlags Flag) const;


	/** Add one flag to one element of BlackboardFlags. */
	void AddBlackboardFlag(const FName& EntryName, ESP_AIBBPlanGenFlags Flag);

	/** Add multiple flags to one element of BlackboardFlags. */
	template <typename... Args>
	void AddBlackboardFlags(const FName& EntryName, Args... OtherFlags)
	{
		AddBlackboardFlags_Internal(BlackboardFlags.FindOrAdd(EntryName), OtherFlags...);
	}


	/** Remove one flag to one element of BlackboardFlags. */
	void RemoveBlackboardFlag(const FName& EntryName, ESP_AIBBPlanGenFlags Flag);

	/** Remove multiple flags to one element of BlackboardFlags. */
	template <typename... Args>
	void RemoveBlackboardFlags(const FName& EntryName, Args... OtherFlags)
	{
		RemoveBlackboardFlags_Internal(BlackboardFlags.FindOrAdd(EntryName), OtherFlags...);
	}
#pragma endregion
};