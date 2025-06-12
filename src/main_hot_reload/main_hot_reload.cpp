#include <SDL.h>
#include <windows.h>
#include <string>
#include <stdio.h>
#include "../game.h"

//#include "../game.cpp"

char* GAME_DLL = "game.dll";

#if DEBUG
#define assert(expression) \
  if (!(expression)) {     \
    *(int*)0 = 0;          \
  }
#else
#define assert(expression)
#endif

const int INITIAL_WIDTH = 640;
const int INITIAL_HEIGHT = 480;

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

GameAPI loadGameAPI(int version) {
    GameAPI api = {};
    api.isValid = false;
    api.version = version;

    api.lastWriteTime = win32GetLastWriteTime(GAME_DLL);

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

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCode) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // TODO: handle error
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow("SDL hot-reload template", INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        // TODO: handle error
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        // TODO: handle error
        exit(1);
    }
    
    printf("Loading game API...\n");
    GameAPI api = loadGameAPI(0);
    if (!api.isValid) {
        //std::cout << "Failed to load game API at startup" << std::endl;
        printf("Failed to tload game API at startup!\n");
        return 1;
    }

    api.initGame(renderer);
    GameInput input = {};
    while (api.isGameRunning()) {
        assert(api.isValid)

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            processEvent(event, &input);
        }

        api.updateGame(renderer, input);

        FILETIME lastWriteTime = win32GetLastWriteTime(GAME_DLL);
        bool reload = CompareFileTime(&lastWriteTime, &api.lastWriteTime) || input.forceReload || input.forceRestart;
        if (reload) {
            printf("Hot-reloading version %d\n", api.version + 1);
            GameAPI newAPI = loadGameAPI(api.version+1);
            if (newAPI.isValid) {
                bool memorySizeChanged = api.getGameMemorySize() != newAPI.getGameMemorySize();
                bool forceRestart = input.forceRestart || memorySizeChanged;
                if (forceRestart) {
                    // Re-initialize the game and all of it's internal state
                    api.releaseGame();
                    newAPI.initGame(renderer);
                } else {
                    // Hot-reload using the current game memory
                    GameMemory *memory = api.getGameMemory();
                    newAPI.hotReloadGame(memory);
                }

                unloadGameAPI(&api);
                api = newAPI;
            } else {
                // TODO: handle failure
                printf("Failed to hot-reload!\n");
            }

            // Reset the input just to be safe (so that we don't get stuck in an infinite reload loop)
            input = {};
        }
    }

    api.releaseGame();
    SDL_DestroyWindow(window);

    return 0;
}