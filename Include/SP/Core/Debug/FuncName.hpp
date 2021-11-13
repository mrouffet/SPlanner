// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CORE_FUNC_NAME_GUARD
#define SPLANNER_CORE_FUNC_NAME_GUARD

/**
*	From MIT Licenced project Sapphire Suite Engine:
*	https://github.com/SapphireSuite/Engine/blob/dev/Include/SA/Core/Debug/Debug.hpp
*/

#if defined(_MSC_VER)

	#define __SP_FUNC_NAME __FUNCSIG__

#elif defined(__GNUC__) || defined(__clang__)

	#define __SP_FUNC_NAME __PRETTY_FUNCTION__

#else

	#define __SP_FUNC_NAME __FUNCTION__

#endif

#endif // GUARD
