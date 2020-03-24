// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/SP_TaskStep.h>

#include <SPlanner/Misc/SP_FlagHelper.h>

#include <SPlanner/AI/Decorator/SP_AIDecorator.h>
#include <SPlanner/AI/Decorator/SP_AIDecoratorFlag.h>

#include <SPlanner/AI/Planner/SP_AIFloatParam.h>
#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_TaskStep::USP_TaskStep(const FObjectInitializer& ObjectInitializer)
{
	TaskInfosClass = USP_TaskInfos::StaticClass();
}

void USP_TaskStep::OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos)
{
	SP_CHECK_NULLPTR(Planner)
	SP_CHECK_NULLPTR(TaskInfos)

	if (!SP_IS_FLAG_SET(NotifyMask, Notify))
		return;

	switch (NotifyAction)
	{
	case ESP_TaskNotification::TNA_TimeOut:
		TaskInfos->BaseExecutionState = ESP_PlanExecutionState::PES_Failed;
		break;
	case ESP_TaskNotification::TNA_WaitForNotify:
		TaskInfos->BaseExecutionState = ESP_PlanExecutionState::PES_Succeed;
		break;
	default:
		SP_LOG(Warning, "TaskNotification [%d] not supported yet!", static_cast<int>(NotifyAction))
		break;
	}
}
void USP_TaskStep::InitNotify(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_CHECK_NULLPTR(Planner)

	// Always bind to allow OnNotify() override.
	Planner->OnNotifyTask.AddDynamic(this, &USP_TaskStep::OnNotify);

	if (!NotifyMask)
		return;

	switch (NotifyAction)
	{
	case ESP_TaskNotification::TNA_TimeOut:
		break;
	case ESP_TaskNotification::TNA_WaitForNotify:
		TaskInfos->BaseExecutionState = ESP_PlanExecutionState::PES_Running;
		break;
	default:
		SP_LOG(Warning, "TaskNotification [%d] not supported yet!", static_cast<int>(NotifyAction))
		break;
	}
}

USP_TaskInfos* USP_TaskStep::InstantiateInfos(UObject* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)
	SP_RCHECK_NULLPTR(TaskInfosClass, nullptr)

	return NewObject<USP_TaskInfos>(Outer, TaskInfosClass);
}

bool USP_TaskStep::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	SP_RCHECK(Cast<USP_AIPlanGenInfos>(Infos), false, "AI Infos nullptr! Infos must be a USP_AIPlanGenInfos!")
	SP_RCHECK(Cast<USP_AIPlannerComponent>(Infos->Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")

	return true;
}
bool USP_TaskStep::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	SP_RCHECK(Cast<USP_AIPlanGenInfos>(Infos), false, "AI Infos nullptr! Infos must be a USP_AIPlanGenInfos!")
	SP_RCHECK(Cast<USP_AIPlannerComponent>(Infos->Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")

	return true;
}
bool USP_TaskStep::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	SP_RCHECK(Cast<USP_AIPlanGenInfos>(Infos), false, "AI Infos nullptr! Infos must be a USP_AIPlanGenInfos!")
	SP_RCHECK(Cast<USP_AIPlannerComponent>(Infos->Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")

	return true;
}

ESP_PlanExecutionState USP_TaskStep::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(TaskInfos, ESP_PlanExecutionState::PES_Failed)

	// Begin Task.
	if (!TaskInfos->bHasBegun && !Begin_Internal(&Planner, TaskInfos))
	{
		// Begin has failed.
		End_Internal(&Planner, TaskInfos); // Call End to uninit.

		return ESP_PlanExecutionState::PES_Failed;
	}

	ESP_PlanExecutionState InternalState = Tick_Internal(DeltaSeconds, &Planner, TaskInfos);

	// Wait for success or failure.
	if (InternalState == ESP_PlanExecutionState::PES_Running)
		return InternalState;
	
	// End Task.
	if (!End_Internal(&Planner, TaskInfos))
		return ESP_PlanExecutionState::PES_Failed; // End has failed.

	// Return either tick has succeeded or failed.
	return InternalState;
}
void USP_TaskStep::Cancel(USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos)
{
	SP_CHECK_NULLPTR(TaskInfos)

	if (TaskInfos->bHasBegun)
	{
		TaskInfos->bForcedEnd = true;
		End_Internal(&Planner, TaskInfos);
	}
}

bool USP_TaskStep::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(TaskInfos, false)


	// Check decorators.
	for (int i = 0; i < Decorators.Num(); ++i)
	{
		SP_CCHECK(Decorators[i], "Decorators[%d] nullptr!", i)

		if (SP_IS_FLAG_SET(Decorators[i]->GetValidateMask(), ESP_AIDecoratorFlag::DF_Begin))
		{
			if (USP_AIDecorator* const AIDecorator = Cast<USP_AIDecorator>(Decorators[i]))
			{
				if (!AIDecorator->Begin_Validate(Planner, TaskInfos))
					return false;
			}
			else if (!Decorators[i]->Validate(Planner))
				return false;
		}
	}


	// Init Notify callback.
	if(TaskInfos->bUseNotify)
		InitNotify(Planner, TaskInfos);

	TaskInfos->bHasBegun = true;

	return true;
}
ESP_PlanExecutionState USP_TaskStep::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)
	SP_RCHECK_NULLPTR(TaskInfos, ESP_PlanExecutionState::PES_Failed)


	// Check decorators.
	for (int i = 0; i < Decorators.Num(); ++i)
	{
		SP_CCHECK(Decorators[i], "Decorators[%d] nullptr!", i)

		if (SP_IS_FLAG_SET(Decorators[i]->GetValidateMask(), ESP_AIDecoratorFlag::DF_Tick))
		{
			if (USP_AIDecorator* const AIDecorator = Cast<USP_AIDecorator>(Decorators[i]))
			{
				if (!AIDecorator->Tick_Validate(DeltaSeconds, Planner, TaskInfos))
					return ESP_PlanExecutionState::PES_Failed;
			}
			else if (!Decorators[i]->Validate(Planner))
				return ESP_PlanExecutionState::PES_Failed;
		}
	}


	return TaskInfos->BaseExecutionState;
}
bool USP_TaskStep::End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)

	if(TaskInfos->bUseNotify && TaskInfos->bHasBegun)
		Planner->OnNotifyTask.RemoveDynamic(this, &USP_TaskStep::OnNotify);

	return true;
}