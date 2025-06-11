#pragma once

#include <SDL.h>

struct GameInput {
    bool quit;
    // Force reload the game dll
    bool forceReload;
    // Force reload the game dll and reset it's state
    bool forceRestart;

    bool moveUp;
    bool moveDown;
    bool moveLeft;
    bool moveRight;
};

void processEvent(SDL_Event event, GameInput *input);