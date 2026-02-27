#include <SDL.h>
#include <SDL_image.h>
#include "game.h"

// TODO: logging
#include <stdio.h>

#define MAX_ENTITIES 1024

#define GRID_OFFSET_X 50
#define GRID_OFFSET_Y 50
#define GRID_ROWS 12
#define GRID_COLUMNS 17
#define TILE_SIZE 32

GameMemory *g_memory;

typedef int EntityIndex;

enum class EntityKind { Nil = 0, Player, Tile };

struct Entity {
    EntityKind kind;

    float x;
    float y;
    float width;
    float height;
    SDL_Texture *texture;

    operator bool() const { return kind != EntityKind::Nil; }
};

struct Entities {
    Entity entities[MAX_ENTITIES];
    EntityIndex count;

    EntityIndex add(EntityKind kind) {
        if (count >= MAX_ENTITIES) {
            printf("Error: Maximum entity count reached!\n");
            return 0;
        }

        if (count == 0) {
            // First entity is the NIL entity, so we skip it
            count++;
        }

        entities[count++].kind = kind;
        return count - 1;
    }

    Entity &get(EntityIndex index) { return entities[index]; }
};

struct GameMemory {
    bool isRunning;
    SDL_Renderer *renderer;

    Entities entities;
    EntityIndex playerIndex;
};

SDL_Renderer *getRenderer() { return g_memory->renderer; }

EntityIndex addTile(int top, int left, int row, int column, int width, int height) {
    SDL_Texture *texture = IMG_LoadTexture(getRenderer(), "assets/yellow_tile.png");
    if (!texture) {
        printf("Failed to load tile texture: %s\n", SDL_GetError());
    }

    EntityIndex index = g_memory->entities.add(EntityKind::Tile);
    Entity &entity = g_memory->entities.get(index);
    if (entity) {
        entity.x = (float)left + column * width;
        entity.y = (float)top + row * height;
        entity.width = (float)width;
        entity.height = (float)height;
        entity.texture = texture;
    }

    return index;
}

EntityIndex addPlayer(int top, int left, int x, int y, int width, int height) {
    SDL_Texture *texture = IMG_LoadTexture(getRenderer(), "assets/lettuce.bmp");
    if (!texture) {
        printf("Failed to load player texture: %s\n", SDL_GetError());
    }

    EntityIndex index = g_memory->entities.add(EntityKind::Player);
    Entity &entity = g_memory->entities.get(index);
    if (entity) {
        entity.x = (float)x;
        entity.y = (float)y;
        entity.width = (float)width;
        entity.height = (float)height;
        entity.texture = texture;
    }

    return index;
}

Entity *getPlayer() { return &g_memory->entities.get(g_memory->playerIndex); }

EXTERN_C
INIT_GAME_API(initGame) {
    g_memory = new GameMemory;
    *g_memory = {};

    g_memory->renderer = renderer;
    g_memory->isRunning = true;

    // Create a grid of tiles
    int gridOffsetX = GRID_OFFSET_X;
    int gridOffsetY = GRID_OFFSET_Y;
    int rows = GRID_ROWS;
    int columns = GRID_COLUMNS;
    int tileWidth = TILE_SIZE;
    int tileHeight = TILE_SIZE;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            addTile(gridOffsetX, gridOffsetY, r, c, tileWidth, tileHeight);
        }
    }

    // Create the player entity
    EntityIndex playerIndex = addPlayer(gridOffsetY, gridOffsetX, 100, 100, 32, 32);
    g_memory->playerIndex = playerIndex;
}

EXTERN_C
HOT_RELOAD_API(hotReloadGame) { g_memory = memory; }

EXTERN_C
IS_GAME_RUNNING_API(isGameRunning) { return g_memory->isRunning; }

EXTERN_C
UPDATE_GAME_API(updateGame) {
    if (input.quit) {
        g_memory->isRunning = false;
        return;
    }

    Entity *player = getPlayer();
    float speed = 0.1f * (float)dt;
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render all entities
    for (int i = 0; i < g_memory->entities.count; i++) {
        Entity &entity = g_memory->entities.get(i);
        SDL_FRect entityRect = {entity.x, entity.y, entity.width, entity.height};
        SDL_RenderTexture(renderer, entity.texture, nullptr, &entityRect);
    }

    SDL_RenderPresent(renderer);
}

EXTERN_C
RELEASE_GAME_API(releaseGame) {
    SDL_DestroyTexture(getPlayer()->texture);
    delete g_memory;
}

EXTERN_C
GET_GAME_MEMORY_API(getGameMemory) { return g_memory; }

EXTERN_C
GET_GAME_MEMORY_SIZE_API(getGameMemorySize) { return sizeof(*g_memory); }