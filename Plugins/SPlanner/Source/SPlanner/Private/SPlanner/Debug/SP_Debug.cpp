// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Debug/SP_Debug.h>

#if SP_DEBUG_LOG
DEFINE_LOG_CATEGORY(LogSP_Debug);
#endif

void USP_Debug::Log(UObject* Caller, const FString& Str, ESP_DebugVerbosity Verbosity)
{
#if SP_DEBUG_LOG
	SP_SCHECK_NULLPTR(Caller)

	switch (Verbosity)
	{
	case ESP_DebugVerbosity::DV_Display:
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		break;
	case ESP_DebugVerbosity::DV_Warning:
		UE_LOG(LogSP_Debug, Warning, TEXT("%s: %s"), *Caller->GetName(), *Str)
		break;
	case ESP_DebugVerbosity::DV_Error:
		UE_LOG(LogSP_Debug, Error, TEXT("%s: %s"), *Caller->GetName(), *Str)
		break;
	default:
		SP_SLOG(Warning, "Unknown verbosity %d", static_cast<int>(Verbosity))
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		break;
	}
#endif
}

void USP_Debug::LogScreen(UObject* Caller, const FString& Str, ESP_DebugVerbosity Verbosity)
{
#if SP_DEBUG_LOG
	SP_SCHECK_NULLPTR(Caller)

	switch (Verbosity)
	{
	case ESP_DebugVerbosity::DV_Display:
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Str);
		break;
	case ESP_DebugVerbosity::DV_Warning:
		UE_LOG(LogSP_Debug, Warning, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Str);
		break;
	case ESP_DebugVerbosity::DV_Error:
		UE_LOG(LogSP_Debug, Error, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Str);
		break;
	default:
		SP_SLOG(Warning, "Unknown verbosity %d", static_cast<int>(Verbosity))
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Str);
		break;
	}
#endif
}

void USP_Debug::LogScreenFull(UObject* Caller, const FString& Str, ESP_DebugVerbosity Verbosity, FLinearColor Color, float Time)
{
#if SP_DEBUG_LOG
	SP_SCHECK_NULLPTR(Caller)

	switch (Verbosity)
	{
	case ESP_DebugVerbosity::DV_Display:
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, Time, Color.ToFColor(true), Str);
		break;
	case ESP_DebugVerbosity::DV_Warning:
		UE_LOG(LogSP_Debug, Warning, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, Time, Color.ToFColor(true), Str);
		break;
	case ESP_DebugVerbosity::DV_Error:
		UE_LOG(LogSP_Debug, Error, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, Time, Color.ToFColor(true), Str);
		break;
	default:
		SP_SLOG(Warning, "Unknown verbosity %d", static_cast<int>(Verbosity))
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, Time, Color.ToFColor(true), Str);
		break;
	}
#endif
}