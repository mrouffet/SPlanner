#pragma once

#include <SPlanner/SP_Config.h>

#if SP_DEBUG

#include <type_traits>

#include <Engine/Engine.h>

#define SP_WFILE_NAME *FString(strrchr(__FILE__, '\\') + 1)
#define SP_WFUNCTION_NAME *FString(__FUNCTION__)

DECLARE_LOG_CATEGORY_EXTERN(LogSP_Debug, Log, All);

/** Log console with object name, file name, line, and function name. */
#define SP_LOG(Verbosity, Str, ...)\
	{\
		if(UFunction* OwnerFunction = this->GetClass()->FindFunctionByName("GetOwner"))\
		{\
			AActor* OwnerActor = nullptr;\
			const_cast<std::remove_cv<std::remove_reference<decltype(*this)>::type>::type&>(*this).ProcessEvent(OwnerFunction, &OwnerActor);\
			if(OwnerActor)\
				UE_LOG(LogSP_Debug, Verbosity, TEXT("Object:%s.%s -- %s:%d in %s:\n\t" Str), *OwnerActor->GetName(), *GetName(), SP_WFILE_NAME, __LINE__, SP_WFUNCTION_NAME, ##__VA_ARGS__)\
			else\
				UE_LOG(LogSP_Debug, Verbosity, TEXT("Object:%s -- %s:%d in %s:\n\t" Str), *GetName(), SP_WFILE_NAME, __LINE__, SP_WFUNCTION_NAME, ##__VA_ARGS__)\
		}\
		else\
			UE_LOG(LogSP_Debug, Verbosity, TEXT("Object:%s -- %s:%d in %s:\n\t" Str), *GetName(), SP_WFILE_NAME, __LINE__, SP_WFUNCTION_NAME, ##__VA_ARGS__)\
	}


/** Log console with object name, file name, line, and function name; and simple screen log. */
#define SP_LOG_SCREEN(Verbosity, Color, Str, ...)\
	{\
		SP_LOG(Verbosity, Str, ##__VA_ARGS__)\
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, FString::Printf(TEXT(Str), ##__VA_ARGS__));\
	}


/** Log console with object name, file name, line, and function name; and screen log. */
#define SP_LOG_SCREEN_FULL(Verbosity, LogKey, Color, Time, Str, ...)\
	{\
		SP_LOG(Verbosity, Str, ##__VA_ARGS__)\
		GEngine->AddOnScreenDebugMessage(LogKey, Time, Color, FString::Printf(TEXT(Str), ##__VA_ARGS__));\
	}

/**
*	Assertion like check.
*	Simple implementation of check.
*/
#define SP_CHECK_FULL(Predicate, PostLogInstruction, Str, ...)\
	if(!(Predicate))\
	{\
		SP_LOG(Error, Str, ##__VA_ARGS__)\
		PostLogInstruction;\
	}


/**
*	Assertion like check.
*	Simple implementation of check.
*/
#define SP_CHECK(Predicate, Str, ...)\
	SP_CHECK_FULL(Predicate, return, Str, ##__VA_ARGS__)

/** Helper check for nullptr objects. */
#define SP_CHECK_NULLPTR(Object, ...) SP_CHECK(Object, #Object " is nullptr!", ##__VA_ARGS__)

/** Helper check for GameState type.*/
#define SP_CHECK_GAMESTATE(GameStateType, ...) SP_CHECK(GetWorld()->GetGameState<GameStateType>(), "Bad GameState type[" #GameStateType "]!", ##__VA_ARGS__)

/** Helper check Authority */
#define SP_CHECK_AUTHORITY(...) SP_CHECK(HasAuthority(), "%s doen't have authority!", *GetName(), ##__VA_ARGS__)

/** Helper check Owner*/
#define SP_CHECK_OWNER(...) SP_CHECK(GetOwner(), "Owner nullptr!", ##__VA_ARGS__)


/**
*	Assertion like check.
*	Implementation of check with return type.
*/
#define SP_RCHECK(Predicate, ReturnValue, Str, ...)\
	SP_CHECK_FULL(Predicate, return ReturnValue, Str, ##__VA_ARGS__)

/** Helper check for nullptr objects. */
#define SP_RCHECK_NULLPTR(Object, ReturnValue, ...) SP_RCHECK(Object, ReturnValue, #Object " is nullptr!", ##__VA_ARGS__)

/** Helper check for GameState type.*/
#define SP_RCHECK_GAMESTATE(GameStateType, ReturnValue, ...) SP_RCHECK(GetWorld()->GetGameState<GameStateType>(), ReturnValue, "Bad GameState type[" #GameStateType "]!", ##__VA_ARGS__)

/** Helper check Authority */
#define SP_RCHECK_AUTHORITY(ReturnValue, ...) SP_RCHECK(HasAuthority(), ReturnValue, "%s doen't have authority!", *GetName(), ##__VA_ARGS__)

/** Helper check Owner*/
#define SP_RCHECK_OWNER(ReturnValue, ...) SP_RCHECK(GetOwner(), ReturnValue, "Owner nullptr!", ##__VA_ARGS__)


/**
*	Assertion like check.
*	Implementation of check with continue.
*/
#define SP_CCHECK(Predicate, Str, ...)\
	SP_CHECK_FULL(Predicate, continue, Str, ##__VA_ARGS__)

/** Helper check for nullptr objects. */
#define SP_CCHECK_NULLPTR(Object, ...) SP_CCHECK(Object, #Object " is nullptr!", ##__VA_ARGS__)

/** Helper check for GameState type.*/
#define SP_CCHECK_GAMESTATE(GameStateType, ...) SP_CCHECK(GetWorld()->GetGameState<GameStateType>(), "Bad GameState type[" #GameStateType "]!", ##__VA_ARGS__)

/** Helper check Authority */
#define SP_CCHECK_AUTHORITY(...) SP_CCHECK(HasAuthority(), "%s doen't have authority!", *GetName(), ##__VA_ARGS__)

/** Helper check Owner*/
#define SP_CCHECK_OWNER(...) SP_CCHECK(GetOwner(), "Owner nullptr!", ##__VA_ARGS__)



/** Static Call */
/** Log console with file name, line, and function name. */
#define SP_SLOG(Verbosity, Str, ...)\
		UE_LOG(LogSP_Debug, Verbosity, TEXT("%s:%d in %s:\n\t" Str), SP_WFILE_NAME, __LINE__, SP_WFUNCTION_NAME, ##__VA_ARGS__)\


/** Log console with file name, line, and function name; and simple screen log. */
#define SP_SLOG_SCREEN(Verbosity, Color, Str, ...)\
	{\
		SP_SLOG(Verbosity, Str, ##__VA_ARGS__)\
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, FString::Printf(TEXT(Str), ##__VA_ARGS__));\
	}


/** Log console with file name, line, and function name; and screen log. */
#define SP_SLOG_SCREEN_FULL(Verbosity, LogKey, Color, Time, Str, ...)\
	{\
		SP_SLOG(Verbosity, Str, ##__VA_ARGS__)\
		GEngine->AddOnScreenDebugMessage(LogKey, Time, Color, FString::Printf(TEXT(Str), ##__VA_ARGS__));\
	}

/**
*	Assertion like check.
*	Simple implementation of check.
*/
#define SP_SCHECK_FULL(Predicate, PostLogInstruction, Str, ...)\
	if(!(Predicate))\
	{\
		SP_SLOG(Error, Str, ##__VA_ARGS__)\
		PostLogInstruction;\
	}

/**
*	Assertion like check.
*	Simple implementation of check.
*/
#define SP_SCHECK(Predicate, Str, ...)\
	SP_SCHECK_FULL(Predicate, return, Str, ##__VA_ARGS__)

/** Helper check for nullptr objects. */
#define SP_SCHECK_NULLPTR(Object, ...) SP_SCHECK(Object, #Object " is nullptr!", ##__VA_ARGS__)

/** Helper check for GameState type.*/
#define SP_SCHECK_GAMESTATE(GameStateType, ...) SP_SCHECK(GetWorld()->GetGameState<GameStateType>(), "Bad GameState type[" #GameStateType "]!", ##__VA_ARGS__)

/** Helper check Authority */
#define SP_SCHECK_AUTHORITY(...) SP_SCHECK(HasAuthority(), "%s doen't have authority!", *GetName(), ##__VA_ARGS__)

/** Helper check Owner*/
#define SP_SCHECK_OWNER(...) SP_SCHECK(GetOwner(), "Owner nullptr!", ##__VA_ARGS__)


/**
*	Assertion like check.
*	Implementation of check with return type.
*/
#define SP_SRCHECK(Predicate, ReturnValue, Str, ...)\
	SP_SCHECK_FULL(Predicate, return ReturnValue, Str, ##__VA_ARGS__)


/** Helper check for nullptr objects. */
#define SP_SRCHECK_NULLPTR(Object, ReturnValue, ...) SP_SRCHECK(Object, ReturnValue, #Object " is nullptr!", ##__VA_ARGS__)

/** Helper check for GameState type.*/
#define SP_SRCHECK_GAMESTATE(GameStateType, ReturnValue, ...) SP_SRCHECK(GetWorld()->GetGameState<GameStateType>(), ReturnValue, "Bad GameState type[" #GameStateType "]!", ##__VA_ARGS__)

/** Helper check Authority */
#define SP_SRCHECK_AUTHORITY(ReturnValue, ...) SP_SRCHECK(HasAuthority(), ReturnValue, "%s doen't have authority!", *GetName(), ##__VA_ARGS__)

/** Helper check Owner*/
#define SP_SRCHECK_OWNER(ReturnValue, ...) SP_SRCHECK(GetOwner(), ReturnValue, "Owner nullptr!", ##__VA_ARGS__)


/**
*	Assertion like check.
*	Implementation of check with continue.
*/
#define SP_SCCHECK(Predicate, Str, ...)\
	SP_SCHECK_FULL(Predicate, continue, Str, ##__VA_ARGS__)


/** Helper check for nullptr objects. */
#define SP_SCCHECK_NULLPTR(Object, ...) SP_SCCHECK(Object, #Object " is nullptr!", ##__VA_ARGS__)

/** Helper check for GameState type.*/
#define SP_SCCHECK_GAMESTATE(GameStateType, ...) SP_SCCHECK(GetWorld()->GetGameState<GameStateType>(), "Bad GameState type[" #GameStateType "]!", ##__VA_ARGS__)

/** Helper check Authority */
#define SP_SCCHECK_AUTHORITY(...) SP_SCCHECK(HasAuthority(), "%s doen't have authority!", *GetName(), ##__VA_ARGS__)

/** Helper check Owner*/
#define SP_SCCHECK_OWNER(...) SP_SCCHECK(GetOwner(), "Owner nullptr!", ##__VA_ARGS__)

#else

#define SP_WFILE_NAME L""
#define SP_WFUNCTION_NAME L""

#define SP_LOG(...)
#define SP_LOG_SCREEN(...)
#define SP_LOG_SCREEN_FULL(...)


#define SP_CHECK_FULL(...)
#define SP_CHECK(...)

#define SP_CHECK_NULLPTR(...)
#define SP_CHECK_GAMESTATE(...)
#define SP_CHECK_AUTHORITY(...)
#define SP_CHECK_OWNER(...)


#define SP_RCHECK(...)

#define SP_RCHECK_NULLPTR(...)
#define SP_RCHECK_GAMESTATE(...)
#define SP_RCHECK_AUTHORITY(...)
#define SP_RCHECK_OWNER(...)


#define SP_CCHECK(...)

#define SP_CCHECK_NULLPTR(...)
#define SP_CCHECK_GAMESTATE(...)
#define SP_CCHECK_AUTHORITY(...)
#define SP_CCHECK_OWNER(...)


#define SP_SLOG(...)
#define SP_SLOG_SCREEN(...)
#define SP_SLOG_SCREEN_FULL(...)


#define SP_SCHECK_FULL(...)
#define SP_SCHECK(...)

#define SP_SCHECK_NULLPTR(...)
#define SP_SCHECK_GAMESTATE(...)
#define SP_SCHECK_AUTHORITY(...)
#define SP_SCHECK_OWNER(...)


#define SP_SRCHECK(...)

#define SP_SRCHECK_NULLPTR(...)
#define SP_SRCHECK_GAMESTATE(...)
#define SP_SRCHECK_AUTHORITY(...)
#define SP_SRCHECK_OWNER(...)


#define SP_SCCHECK(...)

#define SP_SCCHECK_NULLPTR(...)
#define SP_SCCHECK_GAMESTATE(...)
#define SP_SCCHECK_AUTHORITY(...)
#define SP_SCCHECK_OWNER(...)

#endif