:: Init shell if cl is not in the path
echo Checking if cl compiler exists...
WHERE cl
IF %ERRORLEVEL% NEQ 0 call shell

:: Compile target path
set code_path=..\..\src
set lib_folder_path=C:\lib
set sdl_includes=/I%lib_folder_path%\sdl\include /I%lib_folder_path%\sdl\include\SDL3 /I%lib_folder_path%\SDL_image\include\SDL3_image

set sdl_path=%lib_folder_path%\sdl\VisualC\x64\Debug
set sdl_image_path=%lib_folder_path%\SDL_image\VisualC\x64\Debug
set add_sdl_libs=/LIBPATH:%sdl_path% /LIBPATH:%sdl_image_path%

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

:: WIN32 platform required libraries
set win32_libs=                    user32.lib
set win32_libs=%win32_libs%        gdi32.lib
set win32_libs=%win32_libs%        winmm.lib
set win32_libs=%win32_libs%        shell32.lib
set win32_libs=%win32_libs%        SDL3.lib
::set win32_libs=%win32_libs%        SDL3main.lib

:: No optimizations (slow): -Od; all optimizations (fast): -O2
set optimization=                  -Od