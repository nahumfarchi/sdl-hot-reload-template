@echo off

echo Starting debug build...

call scripts\build_common_definitions

set OUT_DIR=build\debug
if not exist %OUT_DIR% mkdir %OUT_DIR%
if not exist %OUT_DIR%\SDL3.dll xcopy ext\SDL3.dll %OUT_DIR%

::odin build source\main_release -out:%OUT_DIR%\game_debug.exe -strict-style -vet -debug
::IF %ERRORLEVEL% NEQ 0 exit /b 1

::xcopy /y /e /i assets %OUT_DIR%\assets > nul
::IF %ERRORLEVEL% NEQ 0 exit /b 1

pushd %OUT_DIR%

:: Note: you can use http://dependencywalker.com/ to check what your exe relies on

:: Compile target path
set code_path=..\..\src
set cpp_files=%code_path%\main_release\main_release.cpp %code_path%\input.cpp

:: WIN32 platform libraries
set win32_libs=                    user32.lib
set win32_libs=%win32_libs%        gdi32.lib
set win32_libs=%win32_libs%        winmm.lib
set win32_libs=%win32_libs%        shell32.lib
set win32_libs=%win32_libs%        SDL3.lib
::set win32_libs=%win32_libs%        SDL3main.lib

:: Build
del *.pdb > NUL 2> NUL
cl %sdl_include% %optimization% %flags% %defines% %debug% %cpp_files% %win32_libs% /link /LIBPATH:%sdl_path% %common_link% %win32_link%

echo Debug build created in %OUT_DIR%

popd