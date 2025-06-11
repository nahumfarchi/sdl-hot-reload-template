#pragma once

#include <SDL.h>

struct GameInput {
    bool quit;

    bool moveUp;
    bool moveDown;
    bool moveLeft;
    bool moveRight;
};

void processEvent(SDL_Event event, GameInput *input);