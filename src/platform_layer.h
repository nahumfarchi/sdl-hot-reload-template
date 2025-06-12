#pragma once

#include <SDL.h>
#include <windows.h>
#include <string>
#include "game.h"

struct GameAPI {
    std::string name;
    int version;
    bool isValid;
    FILETIME lastWriteTime;
    SDL_SharedObject *dll;

    init_game_callback *initGame;
    is_game_running_callback *isGameRunning;
    update_game_callback *updateGame;
    release_game_callback *releaseGame;
    hot_reload_game_callback *hotReloadGame;
    get_game_memory_callback *getGameMemory;
    get_game_memory_size_callback *getGameMemorySize;
};

GameAPI loadGameAPI(const char *dllPath, int version);

void unloadGameAPI(GameAPI *api);

void mainLoopStep(GameAPI api, SDL_Renderer *renderer, GameInput *input);

inline FILETIME win32GetLastWriteTime(char* filename);