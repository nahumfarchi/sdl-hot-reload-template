@echo off

set OUT_DIR=build\hot_reload
if not exist %OUT_DIR% mkdir %OUT_DIR%
if not exist %OUT_DIR%\SDL3.dll xcopy ext\SDL3.dll %OUT_DIR%

::odin build source\main_release -out:%OUT_DIR%\game_debug.exe -strict-style -vet -debug
::IF %ERRORLEVEL% NEQ 0 exit /b 1

::xcopy /y /e /i assets %OUT_DIR%\assets > nul
::IF %ERRORLEVEL% NEQ 0 exit /b 1

pushd %OUT_DIR%

:: Output:
::  win32_handmade.exe
::  handmade.dll

:: Note: you can use http://dependencywalker.com/ to check what your exe relies on

:: Compile target path
set code_path=..\..\src
set cpp_files=%code_path%\main_hot_reload\main_hot_reload.cpp %code_path%\input.cpp
set dll_entry_point=%code_path%\game.cpp
set sdl_path=C:\git\learning\games\sdl-hotreload-template\ext\sdl\VisualC\x64\Debug
set sdl_include=/IC:\git\learning\games\sdl-hotreload-template\ext\sdl\include /IC:\git\learning\games\sdl-hotreload-template\ext\sdl\include\SDL3

:: General compiler flags
set flags=                         -nologo                   &:: Suppress Startup Banner
set flags=%flags%                  -Oi                       &:: Use assembly intrinsics where possible
set flags=%flags%                  -MT                       &:: Include CRT library in the executable (static link)
set flags=%flags%                  -Gm-                      &:: Disable minimal rebuild
set flags=%flags%                  -GR-                      &:: Disable runtime type info (C++)
set flags=%flags%                  -EHa-                     &:: Disable exception handling (C++)
set flags=%flags%                  -W4                       &:: display warnings up to level 4
set flags=%flags%                  -WX                       &:: Treat all warnings as errors
:: Warnings to ignore
set flags=%flags%                  -wd4201                   &:: Nameless struct/union
set flags=%flags%                  -wd4100                   &:: Unused function parameter
set flags=%flags%                  -wd4189                   &:: Local variable not referenced

:: Debug variables
set debug=                         -FC                       &:: Show absolute paths in output
::set debug=%debug%                  -Zi                       &:: Produce debug info (commented out since it can cause problems)
set debug=%debug%                  -Z7                       &:: Produce debug information

:: Cross platform defines
set defines=                       -DDEBUG=1

:: Common linker switches
set common_link=                   -opt:ref                  &:: Remove unused functions
set common_link=%link%             -incremental:no           &:: Make sure we do a complete recompile each time (for hot-reloading)

:: WIN32 linker switches
set win32_link=                    -subsystem:windows,5.2    &:: subsystem, 5.1 for x86 (esnure that we can run in Windows XP)

:: DLL linker switches
set dll_link=                      /EXPORT:initGame          &:: Export the function into the dll's table of functions
set dll_link=%dll_link%            /EXPORT:hotReloadGame
set dll_link=%dll_link%            /EXPORT:isGameRunning
set dll_link=%dll_link%            /EXPORT:updateGame
set dll_link=%dll_link%            /EXPORT:releaseGame
set dll_link=%dll_link%            /EXPORT:getGameMemory
set dll_link=%dll_link%            /EXPORT:getGameMemorySize

:: WIN32 platform required libraries
set win32_libs=                    user32.lib
set win32_libs=%win32_libs%        gdi32.lib
set win32_libs=%win32_libs%        winmm.lib
set win32_libs=%win32_libs%        shell32.lib
set win32_libs=%win32_libs%        SDL3.lib
::set win32_libs=%win32_libs%        SDL3main.lib

:: Game DLL required libraries
set game_dll_libs=                 shell32.lib
set game_dll_libs=%game_dll_libs%  SDL3.lib

:: No optimizations (slow): -Od; all optimizations (fast): -O2
set optimization=                  -Od

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