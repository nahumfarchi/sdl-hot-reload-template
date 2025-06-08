#include <SDL.h>
#include <windows.h>

#include "../game.cpp"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCode) {
    initWindow();
    initGame();
    while (isGameRunning()) {
        updateGame();
    }

    releaseGame();
    closeWindow();

    return 0;
}