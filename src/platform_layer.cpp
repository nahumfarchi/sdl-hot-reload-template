#include "platform_layer.h"

inline FILETIME win32GetLastWriteTime(char* filename) {
    FILETIME lastWriteTime = {};
  
    WIN32_FIND_DATAA findData;
    HANDLE findHandle = FindFirstFileA(filename, &findData);
    if (findHandle != INVALID_HANDLE_VALUE) {
      lastWriteTime = findData.ftLastWriteTime;
      FindClose(findHandle);
    }
  
    return lastWriteTime;
}

GameAPI loadGameAPI(const char *dllPath, int version) {
    GameAPI api = {};
    api.isValid = false;
    api.version = version;

    api.lastWriteTime = win32GetLastWriteTime((char*)dllPath);

    // Create a copy of the dll so that it doesn't become locked
    std::string dllSrc = std::string("game.dll");
    std::string dllDst = std::string("game_") + std::to_string(api.version) + std::string(".dll");
    api.name = dllDst;
    if (!SDL_CopyFile(dllSrc.c_str(), dllDst.c_str())) {
        printf("Failed to copy files from %s to %s", dllSrc.c_str(), dllDst.c_str());
        return api;
    }

    SDL_SharedObject *gameDLL = api.dll = SDL_LoadObject(dllDst.c_str());
    if (!gameDLL) {
        return api;
    }

    api.initGame = (init_game_callback*)SDL_LoadFunction(gameDLL, "initGame");
    if (!api.initGame) {
        return api;
    }

    api.isGameRunning = (is_game_running_callback*)SDL_LoadFunction(gameDLL, "isGameRunning");
    if (!api.isGameRunning) {
        return api;
    }

    api.updateGame = (update_game_callback*)SDL_LoadFunction(gameDLL, "updateGame");
    if (!api.updateGame) {
        return api;
    }

    api.releaseGame = (release_game_callback*)SDL_LoadFunction(gameDLL, "releaseGame");
    if (!api.releaseGame) {
        return api;
    }

    api.hotReloadGame = (hot_reload_game_callback*)SDL_LoadFunction(gameDLL, "hotReloadGame");
    if (!api.hotReloadGame) {
        return api;
    }

    api.getGameMemory = (get_game_memory_callback*)SDL_LoadFunction(gameDLL, "getGameMemory");
    if (!api.getGameMemory) {
        return api;
    }

    api.getGameMemorySize = (get_game_memory_size_callback*)SDL_LoadFunction(gameDLL, "getGameMemorySize");
    if (!api.getGameMemorySize) {
        return api;
    }

    api.isValid = true;
    return api;
}

void unloadGameAPI(GameAPI *api) {
    if (api->dll) {
        SDL_UnloadObject(api->dll);
        api->dll = nullptr;
    }
    
    api->isValid = false;
}

void mainLoopStep(GameAPI api, SDL_Renderer *renderer, GameInput *input) {
    static Uint64 now = SDL_GetPerformanceCounter();
    static Uint64 last = now;

    last = now;
    now = SDL_GetPerformanceCounter();
    double dt = (double)(1000 * (now - last) / (double)SDL_GetPerformanceFrequency());

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        processEvent(event, input);
    }

    api.updateGame(renderer, *input, dt);
}