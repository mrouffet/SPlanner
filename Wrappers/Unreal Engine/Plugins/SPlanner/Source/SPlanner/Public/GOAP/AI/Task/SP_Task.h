// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <Abstract/SP_Object.h>
#include <Abstract/SP_DataAsset.h>

#include <SP/GOAP/AI/Task/ATask.hpp>

#include "SP_TaskState.h"

#include "SP_Task.generated.h"


class USP_TaskData;

/**
 *
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_Task : public USP_DataAsset, public SP::AI::ATask
{
	GENERATED_BODY()

protected:
	/**
	*	The task data class to instantiate.
	*	Value set in constructor in children implementations.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "SPlanner|Task")
	TSubclassOf<USP_TaskData> TaskDataClass;


	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task")
	bool Begin(USP_TaskData* _data) const;

	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task")
	bool End(ESP_TaskState _state, USP_TaskData* _data) const;

	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task")
	ESP_TaskState Tick(float _deltaTime, USP_TaskData* _data) const;


	bool Begin(SP::AI::TaskData* _data) const override final;
	bool End(SP::AI::TaskState _state, SP::AI::TaskData* _data) const override final;
	SP::AI::TaskState Tick(float _deltaTime, SP::AI::TaskData* _data) const override final;

public:
	SP::AI::TaskData* InstantiateData() const override final;
	void DeleteData(SP::AI::TaskData* _data) const override final;
};


UCLASS(ClassGroup = "SPlanner|Task")
class USP_TaskData : public USP_Object, public SP::AI::TaskData
{
	GENERATED_BODY()
};
