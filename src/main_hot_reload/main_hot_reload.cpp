#include <SDL.h>
#include <windows.h>
#include <string>
#include <stdio.h>
#include "../platform_layer.h"
#include "../game.h"

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
    GameAPI api = loadGameAPI(GAME_DLL, 0);
    if (!api.isValid) {
        //std::cout << "Failed to load game API at startup" << std::endl;
        printf("Failed to tload game API at startup!\n");
        return 1;
    }

    GameInput input = {};
    api.initGame(renderer);
    while (api.isGameRunning()) {
        assert(api.isValid);

        mainLoopStep(api, renderer, &input);

        // Hot-reload the game DLL if there's a newer version or if one of
        // the reload shortcut were pressed.
        FILETIME lastWriteTime = win32GetLastWriteTime(GAME_DLL);
        bool reload = CompareFileTime(&lastWriteTime, &api.lastWriteTime) || input.forceReload || input.forceRestart;
        if (reload) {
            printf("Hot-reloading version %d\n", api.version + 1);
            GameAPI newAPI = loadGameAPI(GAME_DLL, api.version+1);
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