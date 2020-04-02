// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Blackboard/SP_BlackboardComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Blackboard/SP_BlackboardAsset.h>

USP_BlackboardComponent::USP_BlackboardComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickInEditor = false;
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
}

USP_ActionSet* USP_BlackboardComponent::GetActionSet(USP_Goal* Goal)
{
	SP_RCHECK_NULLPTR(BlackboardAsset, nullptr)

	return BlackboardAsset->GetActionSet(Goal);
}

void USP_BlackboardComponent::InitializeBlackboard_Implementation()
{
}
void USP_BlackboardComponent::UnInitializeBlackboard_Implementation()
{
}

void USP_BlackboardComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InitializeBlackboard();
}
void USP_BlackboardComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// BlueprintNativeEvent called in UnInitializeComponent cause crashes in Editor.
	UnInitializeBlackboard();
}