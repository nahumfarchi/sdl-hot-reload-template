@echo off

echo Starting hot-reload build...

call scripts\build_common_definitions
call scripts\build_hot_reload_definitions

set OUT_DIR=build\hot_reload
if not exist %OUT_DIR% mkdir %OUT_DIR%
if not exist %OUT_DIR%\SDL3.dll xcopy %sdl_path%\SDL3.dll %OUT_DIR%

::odin build source\main_release -out:%OUT_DIR%\game_debug.exe -strict-style -vet -debug
::IF %ERRORLEVEL% NEQ 0 exit /b 1

::xcopy /y /e /i assets %OUT_DIR%\assets > nul
::IF %ERRORLEVEL% NEQ 0 exit /b 1

pushd %OUT_DIR%

:: Note: you can use http://dependencywalker.com/ to check what your exe relies on

:: Compile target path
set cpp_files=%code_path%\main_hot_reload\main_hot_reload.cpp %code_path%\input.cpp
set dll_entry_point=%code_path%\game.cpp

:: timecmd cl /Zi ..\src\win32_handmade.cpp user32.lib
del *.pdb > NUL 2> NUL
:: TODO - separate the hot-reload build and the platform build
:: Build the game dll
cl %sdl_include% %optimization% %flags% %defines% %debug% -Fmgame.map %dll_entry_point% %game_dll_libs% -LD /link /LIBPATH:%sdl_path% /pdb:game%random%.pdb %common_link% %dll_link%
:: Build the platform layer
:: TODO: -Fmwin32_handmade.map?
cl %sdl_include% %optimization% %flags% %defines% %debug% -Fmmain_hot_reload.map %cpp_files% %win32_libs% /link /LIBPATH:%sdl_path% %common_link% %win32_link%

echo Hot-reload build created in %OUT_DIR%

popd