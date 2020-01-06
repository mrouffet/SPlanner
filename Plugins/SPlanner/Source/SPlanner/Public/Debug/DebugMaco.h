#pragma once

#if !UE_BUILD_SHIPPING

	#define SP_DEBUG 1

	#define SP_DEBUG_EDITOR WITH_EDITOR

#else

	#define SP_DEBUG 0

	#define SP_DEBUG_EDITOR 0

#endif

#if SP_DEBUG

#include <Engine/Engine.h>

#define SP_WFILE_NAME *FString(strrchr(__FILE__, '\\') + 1)
#define SP_WFUNCTION_NAME *FString(__FUNCTION__)

DECLARE_LOG_CATEGORY_EXTERN(LogSP_Debug, Log, All);

/** Log console with file name, line, and function name. */
#define SP_LOG(Verbosity, Str, ...)\
	UE_LOG(LogSP_Debug, Verbosity, TEXT("%s:%d in %s: " Str), SP_WFILE_NAME, __LINE__, SP_WFUNCTION_NAME, ##__VA_ARGS__)


/** Log console with file name, line, and function name; and simple screen log. */
#define SP_LOG_SCREEN(Verbosity, Color, Str, ...)\
	{\
		UE_LOG(LogSP_Debug, Verbosity, TEXT("%s:%d in %s: " Str), SP_WFILE_NAME, __LINE__, SP_WFUNCTION_NAME, ##__VA_ARGS__)\
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, FString::Printf(TEXT(Str), ##__VA_ARGS__));\
	}


/** Log console with file name, line, and function name; and screen log. */
#define SP_LOG_SCREEN_FULL(Verbosity, LogKey, Color, Time, Str, ...)\
	{\
		UE_LOG(LogSP_Debug, Verbosity, TEXT("%s:%d in %s: " Str), SP_WFILE_NAME, __LINE__, SP_WFUNCTION_NAME, ##__VA_ARGS__)\
		GEngine->AddOnScreenDebugMessage(LogKey, Time, Color, FString::Printf(TEXT(Str), ##__VA_ARGS__));\
	}

/**
*	Assertion like check.
*	Simple implementation of check.
*/
#define SP_CHECK(Predicate, Str, ...)\
	if(!(Predicate))\
	{\
		SP_LOG(Error, Str, ##__VA_ARGS__)\
		return;\
	}

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
#define SP_RCHECK(Predicate, Str, ReturnType, ...)\
	if(!(Predicate))\
	{\
		SP_LOG(Error, Str, ##__VA_ARGS__)\
		return ReturnType;\
	}

/** Helper check for nullptr objects. */
#define SP_RCHECK_NULLPTR(Object, ReturnType, ...) SP_RCHECK(Object, #Object " is nullptr!", ReturnType, ##__VA_ARGS__)

/** Helper check for GameState type.*/
#define SP_RCHECK_GAMESTATE(GameStateType, ReturnType, ...) SP_RCHECK(GetWorld()->GetGameState<GameStateType>(), "Bad GameState type[" #GameStateType "]!", ReturnType, ##__VA_ARGS__)

/** Helper check Authority */
#define SP_RCHECK_AUTHORITY(ReturnType, ...) SP_RCHECK(HasAuthority(), "%s doen't have authority!", *GetName(), ReturnType, ##__VA_ARGS__)

/** Helper check Owner*/
#define SP_RCHECK_OWNER(ReturnType, ...) SP_RCHECK(GetOwner(), "Owner nullptr!", ReturnType, ##__VA_ARGS__)


/**
*	Assertion like check.
*	Implementation of check with continue.
*/
#define SP_CCHECK(Predicate, Str, ...)\
	if(!(Predicate))\
	{\
		SP_LOG(Error, Str, ##__VA_ARGS__)\
		continue;\
	}

/** Helper check for nullptr objects. */
#define SP_CCHECK_NULLPTR(Object, ...) SP_CCHECK(Object, #Object " is nullptr!", ##__VA_ARGS__)

/** Helper check for GameState type.*/
#define SP_CCHECK_GAMESTATE(GameStateType, ...) SP_CCHECK(GetWorld()->GetGameState<GameStateType>(), "Bad GameState type[" #GameStateType "]!", ##__VA_ARGS__)

/** Helper check Authority */
#define SP_CCHECK_AUTHORITY(...) SP_CCHECK(HasAuthority(), "%s doen't have authority!", *GetName(), ##__VA_ARGS__)

/** Helper check Owner*/
#define SP_CCHECK_OWNER(...) SP_CCHECK(GetOwner(), "Owner nullptr!", ##__VA_ARGS__)


#else

#define SP_WFILE_NAME L""
#define SP_WFUNCTION_NAME L""

#define SP_LOG(...)
#define SP_LOG_SCREEN(...)
#define SP_LOG_SCREEN_FULL(...)


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

#endif