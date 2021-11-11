// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <Core/Debug/FileName.hpp>

#include <cstring>

/**
*	From MIT Licenced project Sapphire Suite Engine:
*	https://github.com/SapphireSuite/Engine/blob/dev/Source/SA/Core/Debug/Debug.cpp
*/

namespace SP
{
	namespace Intl
	{
		const char* GetFileNameFromPath(const char* _filePath) noexcept
		{
			// Remove characters until last backslash.
			const char* fileName = strrchr(_filePath, '\\');

			if (!fileName) // No backslash found.
				fileName = _filePath;
			else
				fileName += 1; // Remove last '\\' found.


			// Remove characters until last slash.
			if (const char* filePathNoSlash = strrchr(fileName, '/'))
			{
				// Remove last '/' found.
				fileName = filePathNoSlash + 1;
			}

			return fileName;
		}
	}
}
