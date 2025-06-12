
#include "game.h"

GameMemory *g_memory;

EXTERN_C
INIT_GAME_API(initGame) {
    g_memory = new GameMemory;
    *g_memory = {};

    Entity *player = &g_memory->player;
    player->x = 100;
    player->y = 100;
    player->width = 25;
    player->height = 50;

    g_memory->isRunning = true;

    SDL_Surface* bmp = SDL_LoadBMP("assets/lettuce.bmp");
    g_memory->player.texture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_DestroySurface(bmp);
}

EXTERN_C
HOT_RELOAD_API(hotReloadGame) {
    g_memory = memory;
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, player->texture, nullptr, &playerRect);
    SDL_RenderPresent(renderer);
}

EXTERN_C
RELEASE_GAME_API(releaseGame) {
    SDL_DestroyTexture(g_memory->player.texture);
    delete g_memory;
}

EXTERN_C
GET_GAME_MEMORY_API(getGameMemory) {
    return g_memory;
}

EXTERN_C
GET_GAME_MEMORY_SIZE_API(getGameMemorySize) {
    return sizeof(*g_memory);
}