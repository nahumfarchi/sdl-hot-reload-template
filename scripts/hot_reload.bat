@echo off

echo Starting hot-reload...

call scripts\build_common_definitions
call scripts\build_hot_reload_definitions

set OUT_DIR=build\hot_reload
if not exist %OUT_DIR% mkdir %OUT_DIR%
if not exist %OUT_DIR%\SDL3.dll xcopy ext\SDL3.dll %OUT_DIR%
if not exist %OUT_DIR%\SDL3_image.dll xcopy %sdl_image_path%\SDL3_image.dll %OUT_DIR%

::odin build source\main_release -out:%OUT_DIR%\game_debug.exe -strict-style -vet -debug
::IF %ERRORLEVEL% NEQ 0 exit /b 1

::xcopy /y /e /i assets %OUT_DIR%\assets > nul
::IF %ERRORLEVEL% NEQ 0 exit /b 1

pushd %OUT_DIR%

:: Note: you can use http://dependencywalker.com/ to check what your exe relies on

:: Compile target path
set entry_point=%code_path%\main_hot_reload\main_hot_reload.cpp
set dll_entry_point=%code_path%\game.cpp

:: Build the game dll
del *.pdb > NUL 2> NUL
cl %sdl_includes% %optimization% %flags% %defines% %debug% -Fmgame.map %dll_entry_point% %game_dll_libs% -LD /link %add_sdl_libs% /pdb:game%random%.pdb %common_link% %dll_link%

echo Hot-reload build created in %OUT_DIR%

popd