#include <SDL.h>
#include <windows.h>

#include "../platform_layer.h"
#include "../game.cpp"

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

    GameAPI api = {};
    api.name = "release";
    api.isValid = true;
    api.initGame = &initGame;
    api.isGameRunning = &isGameRunning;
    api.updateGame = &updateGame;
    api.releaseGame = &releaseGame;
    api.getGameMemory = &getGameMemory;
    api.getGameMemorySize = &getGameMemorySize;
    
    initGame(renderer);
    GameInput input = {};
    while (isGameRunning()) {
        mainLoopStep(api, renderer, &input);
    }

    releaseGame();
    SDL_DestroyWindow(window);

    return 0;
}