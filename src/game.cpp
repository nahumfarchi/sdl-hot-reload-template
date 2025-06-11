
#include "game.h"

const int INITIAL_WIDTH = 640;
const int INITIAL_HEIGHT = 480;

SDL_Window *g_window;
SDL_Renderer *g_renderer;
GameMemory *g_memory;

EXTERN_C
INIT_WINDOW_API(initWindow) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // TODO: handle error
    }

    g_window = SDL_CreateWindow("SDL hot-reload template", INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!g_window) {
        // TODO: handle error
    }

    g_renderer = SDL_CreateRenderer(g_window, nullptr);
    if (!g_renderer) {
        // TODO: handle error
    }
}

EXTERN_C
INIT_GAME_API(initGame) {
    g_memory = new GameMemory;
    *g_memory = {};

    g_memory->window = g_window;
    g_memory->renderer = g_renderer;

    Entity *player = &g_memory->player;
    player->x = 100;
    player->y = 100;
    player->width = 25;
    player->height = 50;

    g_memory->isRunning = true;
}

EXTERN_C
HOT_RELOAD_API(hotReloadGame) {
    g_memory = memory;
    g_window = memory->window;
    g_renderer = memory->renderer;
}

EXTERN_C
IS_GAME_RUNNING_API(isGameRunning) {
    return g_memory->isRunning;
}

EXTERN_C
UPDATE_GAME_API(updateGame) {
    if (input.quit) {
        g_memory->isRunning = false;
        return;
    }
    
    Entity *player = &g_memory->player;
    float speed = 0.1f;
    if (input.moveUp) {
        player->y -= speed;
    }
    if (input.moveDown) {
        player->y += speed;
    }
    if (input.moveLeft) {
        player->x -= speed;
    }
    if (input.moveRight) {
        player->x += speed;
    }

    SDL_FRect playerRect = { player->x, player->y, player->width, player->height };
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);
    SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
    //SDL_SetRenderDrawColor(g_renderer, 255, 0, 255, 255);
    SDL_RenderRect(g_renderer, &playerRect);
    SDL_RenderPresent(g_renderer);
}

EXTERN_C
RELEASE_GAME_API(releaseGame) {
    delete g_memory;
}

EXTERN_C
CLOSE_WINDOW_API(closeWindow) {
    SDL_DestroyWindow(g_window);
}

EXTERN_C
GET_GAME_MEMORY_API(getGameMemory) {
    return g_memory;
}