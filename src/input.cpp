#include "input.h"

void processKeyDown(SDL_KeyboardEvent event, GameInput *input) {
    if (event.repeat == 0) {
        if (event.scancode == SDL_SCANCODE_UP) {
            input->moveUp = true;
        }

        if (event.scancode == SDL_SCANCODE_DOWN) {
            input->moveDown = true;
        }

        if (event.scancode == SDL_SCANCODE_LEFT) {
            input->moveLeft = true;
        }

        if (event.scancode == SDL_SCANCODE_RIGHT) {
            input->moveRight = true;
        }
    }
}

void processKeyUp(SDL_KeyboardEvent event, GameInput *input) {
    if (event.repeat == 0) {
        if (event.scancode == SDL_SCANCODE_UP) {
            input->moveUp = false;
        }

        if (event.scancode == SDL_SCANCODE_DOWN) {
            input->moveDown = false;
        }

        if (event.scancode == SDL_SCANCODE_LEFT) {
            input->moveLeft = false;
        }

        if (event.scancode == SDL_SCANCODE_RIGHT) {
            input->moveRight = false;
        }
    }
}

void processEvent(SDL_Event event, GameInput *input) {
    SDL_Window *_window = SDL_GetWindowFromEvent(&event);
    SDL_Renderer *_renderer = SDL_GetRenderer(_window);

    switch (event.type) {

        case SDL_EVENT_QUIT: {
            input->quit = true;
            break;
        }

        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
            // printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", event.window.data1, event.window.data2);
            // SDLResizeTexture(renderer, width, height);
            break;
        }

        case SDL_EVENT_WINDOW_EXPOSED: {
            break;
        }

        case SDL_EVENT_KEY_DOWN: {
            processKeyDown(event.key, input);
            break;
        }

        case SDL_EVENT_KEY_UP: {
            processKeyUp(event.key, input);
            break;
        }
    }
}