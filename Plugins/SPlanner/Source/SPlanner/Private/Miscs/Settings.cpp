#include <Miscs/Settings.h>

#if WITH_EDITORONLY_DATA
	static int TaskExecuteKeyOffset = 0;
#endif

uint8 USP_Settings::GetDebugMask()
{
#if WITH_EDITORONLY_DATA
	return GetDefault<USP_Settings>()->DebugMask;
#endif

	return 0;
}
float USP_Settings::GetDebugScreenDisplayTime()
{
#if WITH_EDITORONLY_DATA
	return GetDefault<USP_Settings>()->DebugScreenDisplayTime;
#endif

	return 0.0f;
}

int USP_Settings::GetMoveListLogKey()
{
#if WITH_EDITORONLY_DATA
	return GetDefault<USP_Settings>()->DebugLogKey;
#endif

	return -1;
}
int USP_Settings::GetPlanLogKey()
{
#if WITH_EDITORONLY_DATA
	return GetDefault<USP_Settings>()->DebugLogKey + 1;
#endif

	return -1;
}
int USP_Settings::GetTaskExecuteLogKey()
{
#if WITH_EDITORONLY_DATA
	return GetDefault<USP_Settings>()->DebugLogKey + 2 + TaskExecuteKeyOffset;
#endif

	return -1;
}
void USP_Settings::IncrementTaskExecuteLogKey()
{
#if WITH_EDITORONLY_DATA
	++TaskExecuteKeyOffset;
#endif
}
void USP_Settings::ResetTaskExecuteLogKey()
{
#if WITH_EDITORONLY_DATA
	TaskExecuteKeyOffset = 0;
#endif
}

uint8 USP_Settings::GetMaxPlannerDepth()
{
	return GetDefault<USP_Settings>()->MaxPlannerDepth;
}