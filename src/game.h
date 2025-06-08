#pragma once

#include <SDL.h>

struct Entity {
    float x;
    float y;
    float width;
    float height;
};

struct GameInput {
    bool moveUp;
    bool moveDown;
    bool moveLeft;
    bool moveRight;
};

struct GameMemory {
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool isRunning;
    GameInput lastInput;
    Entity player;
};

#ifdef __cplusplus
// Disable the C++ compiler name-mangling
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

// "Services" that the game provides to the platform layer

#define INIT_WINDOW_API(name) void name()
typedef INIT_WINDOW_API(init_window_callback);

#define INIT_GAME_API(name) void name()
typedef INIT_GAME_API(init_game_callback);

#define HOT_RELOAD_API(name) void name(GameMemory *memory)
typedef HOT_RELOAD_API(hot_reload_game_callback);

#define IS_GAME_RUNNING_API(name) bool name()
typedef IS_GAME_RUNNING_API(is_game_running_callback);

#define UPDATE_GAME_API(name) void name()
typedef UPDATE_GAME_API(update_game_callback);

#define RELEASE_GAME_API(name) void name()
typedef RELEASE_GAME_API(release_game_callback);

#define CLOSE_WINDOW_API(name) void name()
typedef CLOSE_WINDOW_API(close_window_callback);

#define GET_GAME_MEMORY_API(name) GameMemory* name()
typedef GET_GAME_MEMORY_API(get_game_memory_callback);

void processEvent(SDL_Event event, GameInput *input);
void processKeyDown(SDL_KeyboardEvent event, GameInput *input);
void processKeyUp(SDL_KeyboardEvent event, GameInput *input);