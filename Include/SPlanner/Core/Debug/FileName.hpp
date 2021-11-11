// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CORE_FILE_NAME_GUARD
#define SPLANNER_CORE_FILE_NAME_GUARD

/**
*	From MIT Licenced project Sapphire Suite Engine:
*	https://github.com/SapphireSuite/Engine/blob/dev/Include/SA/Core/Debug/Debug.hpp
*/

namespace SP
{
	namespace Intl
	{
		const char* GetFileNameFromPath(const char* _filePath) noexcept;
	}

}

#define __SP_FILE_NAME SP::Intl::GetFileNameFromPath(__FILE__)

#endif // GUARD
