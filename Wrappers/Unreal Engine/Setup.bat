:: Copyright (c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

setlocal


:: Setup C++ library into SPlanner UE plugin.

set pluginSourcePath=Plugins\SPlanner\Source\SPlanner

:: Copy SPlanner Lib includes.
echo d | xcopy "..\..\Include" "%pluginSourcePath%\Public" /E /Y

:: Copy SPlanner Lib sources.
echo d | xcopy "..\..\Source" "%pluginSourcePath%\Private" /E /Y


:: Build & Open Unreal Engine project.

:: Query Unreal Engine project generator process (same as right click on .upoject file -> "Generate Visual Studio project files").
:: Source: https://answers.unrealengine.com/questions/14280/generate-vs-project-files-by-command-line.html 
powershell -command "& { (Get-ItemProperty 'Registry::HKEY_CLASSES_ROOT\Unreal.ProjectFile\shell\rungenproj' -Name 'Icon' ).'Icon' }" > gen_temp.txt

:: Set registery output in variable.
set /p gen=<gen_temp.txt
del /q gen_temp.txt


:: Generate Visual Studio project files.
%gen% /projectfiles "%cd%\SPlannerDemo.uproject"

:: launch Visual Studio.
start SPlannerDemo.sln

:: Launch Unreal Engine Editor.
%gen% /editor "%cd%\SPlannerDemo.uproject"
