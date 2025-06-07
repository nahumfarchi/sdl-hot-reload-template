#include "SDL.h"

struct Entity {
    float x;
    float y;
    float width;
    float height;
};

struct GameInput {
    bool moveUp;
    bool moveDown;
    bool moveLeft;
    bool moveRight;
};

struct GameMemory {
    GameInput lastInput;
    Entity player;
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

    SDL_Window *window;
    SDL_Renderer *renderer;
    bool _isRunning = true;

    void processEvent(SDL_Event event, GameInput *input);
    void processKeyDown(SDL_KeyboardEvent event, GameInput *input);
    void processKeyUp(SDL_KeyboardEvent event, GameInput *input);
};