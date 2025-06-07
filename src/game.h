#include "SDL.h"

struct GameMemory {

};

class Game {
    public:

    GameMemory *memory;

    void initWindow();
    void initGame();
    bool isRunning();
    void update();
    void closeGame();
    void closeWindow();

    private:

    bool _isRunning = true;

    void handleEvent(SDL_Event event);
};