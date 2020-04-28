// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/SP_TaskImpl.h>

#include <SPlanner/Misc/SP_FlagHelper.h>

#include <SPlanner/AI/Decorator/SP_AIPlannerDecorator.h>
#include <SPlanner/AI/Decorator/SP_AIPlannerDecoratorFlag.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_TaskImpl::USP_TaskImpl(const FObjectInitializer& ObjectInitializer)
{
	TaskInfosClass = USP_TaskInfos::StaticClass();
}

void USP_TaskImpl::OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos)
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
void USP_TaskImpl::InitNotify(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_CHECK_NULLPTR(Planner)

	// Always bind to allow OnNotify() override.
	Planner->OnNotifyTask.AddDynamic(this, &USP_TaskImpl::OnNotify);

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

USP_TaskInfos* USP_TaskImpl::InstantiateInfos(UObject* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)
	SP_RCHECK_NULLPTR(TaskInfosClass, nullptr)

	return NewObject<USP_TaskInfos>(Outer, TaskInfosClass);
}

bool USP_TaskImpl::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	SP_RCHECK(Cast<USP_AIPlanGenInfos>(Infos), false, "AI Infos nullptr! Infos must be a USP_AIPlanGenInfos!")
	SP_RCHECK(Cast<USP_AIPlannerComponent>(Infos->Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")

	return true;
}
bool USP_TaskImpl::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	SP_RCHECK(Cast<USP_AIPlanGenInfos>(Infos), false, "AI Infos nullptr! Infos must be a USP_AIPlanGenInfos!")
	SP_RCHECK(Cast<USP_AIPlannerComponent>(Infos->Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")

	return true;
}
bool USP_TaskImpl::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	SP_RCHECK(Cast<USP_AIPlanGenInfos>(Infos), false, "AI Infos nullptr! Infos must be a USP_AIPlanGenInfos!")
	SP_RCHECK(Cast<USP_AIPlannerComponent>(Infos->Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")

	return true;
}

ESP_PlanExecutionState USP_TaskImpl::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos)
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
void USP_TaskImpl::Cancel(USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos)
{
	SP_CHECK_NULLPTR(TaskInfos)

	if (TaskInfos->bHasBegun)
	{
		TaskInfos->bForcedEnd = true;
		End_Internal(&Planner, TaskInfos);

		PostExecution(&Planner, false);
	}
}

bool USP_TaskImpl::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(TaskInfos, false)


	// Check decorators.
	for (int i = 0; i < Decorators.Num(); ++i)
	{
		SP_CCHECK(Decorators[i], "Decorators[%d] nullptr!", i)

		if (SP_IS_FLAG_SET(Decorators[i]->GetValidateMask(), ESP_AIPlannerDecoratorFlag::DF_Begin))
		{
			// Try cast as USP_PlannerDecorator.
			if (USP_AIPlannerDecorator* const AIPlannerDecorator = Cast<USP_AIPlannerDecorator>(Decorators[i]))
			{
				if(!AIPlannerDecorator->Begin_Validate(Planner, TaskInfos))
					return false;
			}
			else if(Decorators[i]->Validate(Planner)) // Try base validate using Planner.
				return false;
		}
	}


	// Init Notify callback.
	if(TaskInfos->bUseNotify)
		InitNotify(Planner, TaskInfos);

	TaskInfos->bHasBegun = true;

	return true;
}
ESP_PlanExecutionState USP_TaskImpl::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)
	SP_RCHECK_NULLPTR(TaskInfos, ESP_PlanExecutionState::PES_Failed)


	// Check decorators.
	for (int i = 0; i < Decorators.Num(); ++i)
	{
		SP_CCHECK(Decorators[i], "Decorators[%d] nullptr!", i)

		if (SP_IS_FLAG_SET(Decorators[i]->GetValidateMask(), ESP_AIPlannerDecoratorFlag::DF_Tick))
		{
			// Try cast as USP_PlannerDecorator.
			if (USP_AIPlannerDecorator* const AIPlannerDecorator = Cast<USP_AIPlannerDecorator>(Decorators[i]))
			{
				if(!AIPlannerDecorator->Tick_Validate(DeltaSeconds, Planner, TaskInfos))
					return ESP_PlanExecutionState::PES_Failed;
			}
			else if (Decorators[i]->Validate(Planner)) // Try base validate using Planner.
				return ESP_PlanExecutionState::PES_Failed;
		}
	}


	return TaskInfos->BaseExecutionState;
}
bool USP_TaskImpl::End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)

	if(TaskInfos->bUseNotify && TaskInfos->bHasBegun)
		Planner->OnNotifyTask.RemoveDynamic(this, &USP_TaskImpl::OnNotify);

	return true;
}

void USP_TaskImpl::Freeze_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
}
void USP_TaskImpl::UnFreeze_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
}