:: Copyright (c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

:: Kill Unreal Engine and visual studio process
taskkill /f /im UE4Editor.exe
taskkill /f /im devenv.exe

::Wait for kills.
timeout 3 /nobreak

:: Clean files
rmdir /s /q .vs
rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q Saved
rmdir /s /q DerivedDataCache

rmdir /s /q Plugins\SPlanner\Binaries
rmdir /s /q Plugins\SPlanner\Intermediate

:: C++ library files.
rmdir /s /q Plugins\SPlanner\Source\SPlanner\Public\SP
rmdir /s /q Plugins\SPlanner\Source\SPlanner\Private\SP

del /s /q *.sln
