#include <SDL.h>
#include <windows.h>

#include "../game.cpp"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCode) {
    SDL_ShowSimpleMessageBox(0, "The title", "The message 3", 0);
    
    Game game;
    game.initWindow();
    game.initGame();
    while (game.isRunning()) {
        game.update();
    }

    game.closeGame();
    game.closeWindow();

    return 0;
}