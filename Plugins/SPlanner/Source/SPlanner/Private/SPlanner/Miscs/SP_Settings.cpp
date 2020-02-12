#include <SPlanner/Miscs/SP_Settings.h>

#if WITH_EDITOR
static int TaskExecuteKeyOffset = 0;

uint8 USP_Settings::GetDebugMask()
{
	return GetDefault<USP_Settings>()->DebugMask;
}
float USP_Settings::GetDebugScreenDisplayTime()
{
	return GetDefault<USP_Settings>()->DebugScreenDisplayTime;
}

int USP_Settings::GetActionListLogKey()
{
	return GetDefault<USP_Settings>()->DebugLogKey;
}
int USP_Settings::GetPlanLogKey()
{
	return GetDefault<USP_Settings>()->DebugLogKey + 1;
}
int USP_Settings::GetPlanGenerationLogKey()
{
	return GetDefault<USP_Settings>()->DebugLogKey + 2;
}
int USP_Settings::GetTaskExecuteLogKey()
{
	return GetDefault<USP_Settings>()->DebugLogKey + 3 + TaskExecuteKeyOffset;
}
void USP_Settings::IncrementTaskExecuteLogKey()
{
	++TaskExecuteKeyOffset;
}
void USP_Settings::ResetTaskExecuteLogKey()
{
	TaskExecuteKeyOffset = 0;
}
#endif