
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

    GameInput *input = &g_memory->lastInput;
    *input = {};

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
    SDL_Event event;
    GameInput *input = &g_memory->lastInput;
    while (SDL_PollEvent(&event)) {
        processEvent(event, input);
    }

    Entity *player = &g_memory->player;
    float speed = 0.1f;
    if (input->moveUp) {
        player->y -= speed;
    }
    if (input->moveDown) {
        player->y += speed;
    }
    if (input->moveLeft) {
        player->x -= speed;
    }
    if (input->moveRight) {
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

void processEvent(SDL_Event event, GameInput *input) {
    SDL_Window *_window = SDL_GetWindowFromEvent(&event);
    SDL_Renderer *_renderer = SDL_GetRenderer(_window);

    int width, height;
    SDL_GetWindowSize(_window, &width, &height);

    switch (event.type) {

        case SDL_EVENT_QUIT: {
            g_memory->isRunning = false;
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