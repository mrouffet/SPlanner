// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Director/SP_Director.h>
#include "SP_AIDirector.generated.h"

class USP_FormationSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_AIDirectorNumDelegate, int, AINum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_AIDirectorRegisterFormationSetDelegate, USP_FormationSet*, InFormationSet);

UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Director")
class SPLANNER_API ASP_AIDirector : public ASP_Director
{
	GENERATED_BODY()

protected:
	/** All formation registered. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_FormationSet*> FormationSets;

	/** Active AI number. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	int ActiveAINum = 0;

	void OnRegistedPlannerActive(USP_PlannerComponent* InPlanner) override;
	void OnRegistedPlannerInactive(USP_PlannerComponent* InPlanner) override;

	/**
	*	Implementation of formation set register.
	*	Called by static RegisterFormationSet().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Director|AI")
	void RegisterFormationSet_Internal(USP_FormationSet* InFormationSet);

	/**
	*	Implementation of formation set unregister.
	*	Called by static UnRegisterFormationSet().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Director|AI")
	void UnRegisterFormationSet_Internal(USP_FormationSet* InFormationSet);

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** Callback event when AI get active or inactive. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Director|AI")
	FSP_AIDirectorNumDelegate OnAINumChange;

	/** Callback event when a formation set get registered. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Director|AI")
	FSP_AIDirectorRegisterFormationSetDelegate OnFormationSetRegistered;

	/** Callback event when a formation set get unregistered. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Director|AI")
	FSP_AIDirectorRegisterFormationSetDelegate OnFormationSetUnRegistered;


	ASP_AIDirector(const FObjectInitializer& ObjectInitializer);

	/** Getter of FormationSets. */
	const TArray<USP_FormationSet*>& GetFormationSets() const;

	void Tick(float DeltaSeconds) override;

	/**
	*	Register InFormationSet in Director instance.
	*	Require Director actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static void RegisterFormationSet(USP_FormationSet* InFormationSet);

	/**
	*	Unregister InFormationSet in Director instance.
	*	Require Director actor in scene, throw error otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	static void UnRegisterFormationSet(USP_FormationSet* InFormationSet);

	/**
	*	Freeze all selected planner in editor.
	*	In editor, only selected planner will be frozen (or all planner if no selected and bApplyToAllIfNoSelected).
	*	In non SP_DEBUG build, every registered planner will be frozen.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	void FreezeAllSelectedPlanner(float Time = -1.0f, bool bApplyToAllIfNoSelected = true);

	/**
	*	UnFreeze all selected planner in editor.
	*	In editor, only selected planner will be unfrozen (or all planner if no selected and bApplyToAllIfNoSelected).
	*	In non SP_DEBUG build, every registered planner will be unfrozen.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Director")
	void UnFreezeAllSelectedPlanner(bool bApplyToAllIfNoSelected = true);
};