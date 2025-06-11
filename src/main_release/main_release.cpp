#include <SDL.h>
#include <windows.h>

#include "../game.cpp"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCode) {
    initWindow();
    initGame();
    GameInput input = {};
    while (isGameRunning()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            processEvent(event, &input);
        }

        updateGame(input);
    }

    releaseGame();
    closeWindow();

    return 0;
}