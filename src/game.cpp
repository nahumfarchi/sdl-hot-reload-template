
#include "game.h"

const int INITIAL_WIDTH = 640;
const int INITIAL_HEIGHT = 480;

SDL_Window *g_window;
SDL_Renderer *g_renderer;

void Game::initWindow() {
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

void Game::initGame() {
    this->memory = new GameMemory;
}

bool Game::isRunning() {
    return this->_isRunning;
}

void Game::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        this->handleEvent(event);
    }
}

void Game::closeGame() {
    delete this->memory;
}

void Game::closeWindow() {
    SDL_DestroyWindow(g_window);
}

void Game::handleEvent(SDL_Event event) {
    SDL_Window *window = SDL_GetWindowFromEvent(&event);
    SDL_Renderer *renderer = SDL_GetRenderer(window);

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    switch (event.type) {
    case SDL_EVENT_QUIT: {
        this->_isRunning = false;
    } break;

    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
        // printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", event.window.data1, event.window.data2);
        // SDLResizeTexture(renderer, width, height);
    } break;

    case SDL_EVENT_WINDOW_EXPOSED: {
    } break;
    }
}