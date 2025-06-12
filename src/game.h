#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "input.h"

struct Entity {
    float x;
    float y;
    float width;
    float height;
    SDL_Texture *texture;
};

struct GameMemory {
    bool isRunning;
    Entity player;
};

#ifdef __cplusplus
// Disable the C++ compiler name-mangling
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

// "Services" that the game provides to the platform layer

#define INIT_GAME_API(name) void name(SDL_Renderer *renderer)
typedef INIT_GAME_API(init_game_callback);

#define HOT_RELOAD_API(name) void name(GameMemory *memory)
typedef HOT_RELOAD_API(hot_reload_game_callback);

#define IS_GAME_RUNNING_API(name) bool name()
typedef IS_GAME_RUNNING_API(is_game_running_callback);

#define UPDATE_GAME_API(name) void name(SDL_Renderer *renderer, GameInput input, double dt)
typedef UPDATE_GAME_API(update_game_callback);

#define RELEASE_GAME_API(name) void name()
typedef RELEASE_GAME_API(release_game_callback);

#define GET_GAME_MEMORY_API(name) GameMemory* name()
typedef GET_GAME_MEMORY_API(get_game_memory_callback);

#define GET_GAME_MEMORY_SIZE_API(name) int name()
typedef GET_GAME_MEMORY_SIZE_API(get_game_memory_size_callback);

void processEvent(SDL_Event event, GameInput *input);
void processKeyDown(SDL_KeyboardEvent event, GameInput *input);
void processKeyUp(SDL_KeyboardEvent event, GameInput *input);