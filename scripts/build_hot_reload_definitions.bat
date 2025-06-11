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