:: Kill Unreal Engine and visual studio process
TASKKILL /f /im UE4Editor.exe
TASKKILL /f /im devenv.exe

::Wait for kills.
TIMEOUT 3 /nobreak

:: Clean files
RMDIR /s /q .vs
RMDIR /s /q Binaries
RMDIR /s /q Intermediate
RMDIR /s /q Saved

RMDIR /s /q Plugins\SPlanner\Binaries
RMDIR /s /q Plugins\SPlanner\Intermediate

DEL /s /q *.sln