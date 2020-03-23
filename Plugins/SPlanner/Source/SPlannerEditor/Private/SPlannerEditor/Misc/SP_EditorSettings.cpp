// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlannerEditor/Misc/SP_EditorSettings.h>

static int TaskExecuteKeyOffset = 0;

uint8 USP_EditorSettings::GetDebugMask()
{
	return GetDefault<USP_EditorSettings>()->DebugMask;
}
float USP_EditorSettings::GetDebugScreenDisplayTime()
{
	return GetDefault<USP_EditorSettings>()->DebugScreenDisplayTime;
}

int USP_EditorSettings::GetActionListLogKey()
{
	return GetDefault<USP_EditorSettings>()->DebugLogKey;
}
int USP_EditorSettings::GetPlanLogKey()
{
	return GetDefault<USP_EditorSettings>()->DebugLogKey + 1;
}
int USP_EditorSettings::GetPlanGenerationLogKey()
{
	return GetDefault<USP_EditorSettings>()->DebugLogKey + 2;
}
int USP_EditorSettings::GetTaskExecuteLogKey()
{
	return GetDefault<USP_EditorSettings>()->DebugLogKey + 3 + TaskExecuteKeyOffset;
}
void USP_EditorSettings::IncrementTaskExecuteLogKey()
{
	++TaskExecuteKeyOffset;
}
void USP_EditorSettings::ResetTaskExecuteLogKey()
{
	TaskExecuteKeyOffset = 0;
}