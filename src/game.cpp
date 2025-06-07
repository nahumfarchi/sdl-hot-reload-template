
#include "game.h"

const int INITIAL_WIDTH = 640;
const int INITIAL_HEIGHT = 480;

void Game::initWindow() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // TODO: handle error
    }

    this->window = SDL_CreateWindow("SDL hot-reload template", INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!this->window) {
        // TODO: handle error
    }

    this->renderer = SDL_CreateRenderer(this->window, nullptr);
    if (!this->renderer) {
        // TODO: handle error
    }
}

void Game::initGame() {
    this->memory = new GameMemory;
    Entity *player = &this->memory->player;
    player->x = 100;
    player->y = 100;
    player->width = 25;
    player->height = 50;
}

bool Game::isRunning() {
    return this->_isRunning;
}

void Game::update() {
    SDL_Event event;
    GameInput *input = &this->memory->lastInput;
    while (SDL_PollEvent(&event)) {
        this->processEvent(event, input);
    }

    Entity *player = &this->memory->player;
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
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
    SDL_SetRenderDrawColor(this->renderer, 255, 0, 255, 255);
    SDL_RenderRect(this->renderer, &playerRect);
    SDL_RenderPresent(this->renderer);
}

void Game::closeGame() {
    delete this->memory;
}

void Game::closeWindow() {
    SDL_DestroyWindow(this->window);
}

void Game::processEvent(SDL_Event event, GameInput *input) {
    SDL_Window *_window = SDL_GetWindowFromEvent(&event);
    SDL_Renderer *_renderer = SDL_GetRenderer(_window);

    int width, height;
    SDL_GetWindowSize(_window, &width, &height);

    switch (event.type) {

        case SDL_EVENT_QUIT: {
            this->_isRunning = false;
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
            this->processKeyDown(event.key, input);
            break;
        }

        case SDL_EVENT_KEY_UP: {
            this->processKeyUp(event.key, input);
            break;
        }
    }
}

void Game::processKeyDown(SDL_KeyboardEvent event, GameInput *input) {
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

void Game::processKeyUp(SDL_KeyboardEvent event, GameInput *input) {
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