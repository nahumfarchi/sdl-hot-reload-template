@echo off

echo Starting debug build...

call scripts\build_common_definitions

set OUT_DIR=build\debug
if not exist %OUT_DIR% mkdir %OUT_DIR%
if not exist %OUT_DIR%\SDL3.dll xcopy ext\SDL3.dll %OUT_DIR%
if not exist %OUT_DIR%\SDL3_image.dll xcopy %sdl_image_path%\SDL3_image.dll %OUT_DIR%

echo Copying assets...
xcopy /y /e /i assets %OUT_DIR%\assets > nul
IF %ERRORLEVEL% NEQ 0 exit /b 1

::odin build source\main_release -out:%OUT_DIR%\game_debug.exe -strict-style -vet -debug
::IF %ERRORLEVEL% NEQ 0 exit /b 1

pushd %OUT_DIR%

:: Note: you can use http://dependencywalker.com/ to check what your exe relies on

:: Compile target path
set code_path=..\..\src
set cpp_files=%code_path%\main_release\main_release.cpp %code_path%\input.cpp %code_path%\platform_layer.cpp

:: Build
Echo Building...
del *.pdb > NUL 2> NUL
cl %sdl_includes% %optimization% %flags% %defines% %debug% %cpp_files% %win32_libs% /link %add_sdl_libs% %common_link% %win32_link%

echo Debug build created in %OUT_DIR%

popd