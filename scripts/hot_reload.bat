@echo off

echo Starting hot-reload...

call build_common_definitions

set OUT_DIR=build\hot_reload
if not exist %OUT_DIR% mkdir %OUT_DIR%
if not exist %OUT_DIR%\SDL3.dll xcopy ext\SDL3.dll %OUT_DIR%

::odin build source\main_release -out:%OUT_DIR%\game_debug.exe -strict-style -vet -debug
::IF %ERRORLEVEL% NEQ 0 exit /b 1

::xcopy /y /e /i assets %OUT_DIR%\assets > nul
::IF %ERRORLEVEL% NEQ 0 exit /b 1

pushd %OUT_DIR%

:: Note: you can use http://dependencywalker.com/ to check what your exe relies on

:: Compile target path
set entry_point=%code_path%\main_hot_reload\main_hot_reload.cpp
set dll_entry_point=%code_path%\game.cpp

:: DLL linker switches
set dll_link=                      /EXPORT:initGame          &:: Export the function into the dll's table of functions
set dll_link=%dll_link%            /EXPORT:hotReloadGame
set dll_link=%dll_link%            /EXPORT:isGameRunning
set dll_link=%dll_link%            /EXPORT:updateGame
set dll_link=%dll_link%            /EXPORT:releaseGame
set dll_link=%dll_link%            /EXPORT:getGameMemory
set dll_link=%dll_link%            /EXPORT:getGameMemorySize

:: Game DLL required libraries
set game_dll_libs=                 shell32.lib
set game_dll_libs=%game_dll_libs%  SDL3.lib

:: Build the game dll
del *.pdb > NUL 2> NUL
cl %sdl_include% %optimization% %flags% %defines% %debug% -Fmgame.map %dll_entry_point% %game_dll_libs% -LD /link /LIBPATH:%sdl_path% /pdb:game%random%.pdb %common_link% %dll_link%

echo Hot-reload build created in %OUT_DIR%

popd