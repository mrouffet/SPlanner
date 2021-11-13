// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CORE_FILE_NAME_GUARD
#define SPLANNER_CORE_FILE_NAME_GUARD

/**
*	\file FileName.hpp
* 
*	\brief Get current file name preprocessor definition.
* 
*	From MIT Licenced project Sapphire Suite Engine:
*	https://github.com/SapphireSuite/Engine/blob/dev/Include/SA/Core/Debug/Debug.hpp
* 
*	\ingroup Debug
*	\{
*/

namespace SP
{
	/// \cond Internal

	namespace Intl
	{
		/**
		*	\brief Get file name from path.
		* 
		*	\param[in] _filePath	file path.
		* 
		*	\return file name.
		*/
		const char* GetFileNameFromPath(const char* _filePath) noexcept;
	}

	/// \endcond
}

/// Preprocessor for current file name.
#define __SP_FILE_NAME SP::Intl::GetFileNameFromPath(__FILE__)


/** \} */

#endif // GUARD
