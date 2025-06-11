# SDL Hot-Reload Template

An SDL hot-reload template for game development in C++.

## Setup

1. Install MSVC build tools (in https://visualstudio.microsoft.com/downloads/ under "Tools for Visual Studio")
2. Run `shell.bat` from the command line (this will add all of the scripts to the path)
3. Update sdl_path and sdl_include path in scripts/build_common_definitions.bat to point at your SDL .dll and .h files (https://github.com/libsdl-org/SDL)
4. Run `build.bat` from the command line

## Dev loop

1. `run.bat`
2. `reload.bat`

## Debug with Visual Studio

Simply run `debug.bat`. This will create and open a VS solution from which you can debug. Hot-reloading works here as well.

---

Inspired by https://hero.handmade.network/episode/code/day021/ and https://github.com/karl-zylinski/odin-raylib-hot-reload-game-template
