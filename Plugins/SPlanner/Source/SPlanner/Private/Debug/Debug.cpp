#include <Debug/Debug.h>

DEFINE_LOG_CATEGORY(LogSP_Debug);

void USP_Debug::Log(UObject* Caller, const FString& Str, ESP_DebugVerbosity Verbosity)
{
	SP_CHECK_NULLPTR(Caller)

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
		SP_LOG(Warning, "Unknown verbosity %d", static_cast<int>(Verbosity))
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		break;
	}

}

void USP_Debug::LogScreen(UObject* Caller, const FString& Str, ESP_DebugVerbosity Verbosity)
{
	SP_CHECK_NULLPTR(Caller)

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
		SP_LOG(Warning, "Unknown verbosity %d", static_cast<int>(Verbosity))
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Str);
		break;
	}
}

void USP_Debug::LogScreenColor(UObject* Caller, const FString& Str, ESP_DebugVerbosity Verbosity, const FColor& Color)
{
	SP_CHECK_NULLPTR(Caller)

	switch (Verbosity)
	{
	case ESP_DebugVerbosity::DV_Display:
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, Str);
		break;
	case ESP_DebugVerbosity::DV_Warning:
		UE_LOG(LogSP_Debug, Warning, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, Str);
		break;
	case ESP_DebugVerbosity::DV_Error:
		UE_LOG(LogSP_Debug, Error, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, Str);
		break;
	default:
		SP_LOG(Warning, "Unknown verbosity %d", static_cast<int>(Verbosity))
		UE_LOG(LogSP_Debug, Display, TEXT("%s: %s"), *Caller->GetName(), *Str)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, Str);
		break;
	}
}