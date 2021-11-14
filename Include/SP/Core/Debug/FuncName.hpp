// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CORE_FUNC_NAME_GUARD
#define SPLANNER_CORE_FUNC_NAME_GUARD

/**
*	\file FuncName.hpp
* 
*	\brief Get current function name preprocessor definition.
* 
*	From MIT Licenced project Sapphire Suite Engine:
*	https://github.com/SapphireSuite/Engine/blob/dev/Include/SA/Core/Debug/Debug.hpp
* 
*	\ingroup Debug
*	\{
*/

#if defined(DOXYGEN)

	/// Preprocessor for current function name.
	#define __SP_FUNC_NAME

#elif defined(_MSC_VER)

	#define __SP_FUNC_NAME __FUNCSIG__

#elif defined(__GNUC__) || defined(__clang__)

	#define __SP_FUNC_NAME __PRETTY_FUNCTION__

#else

	#define __SP_FUNC_NAME __FUNCTION__

#endif


/** \} */

#endif // GUARD
