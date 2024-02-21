#include "Game.h"
#include <iostream>

using namespace std;

//Window dimenstions and game state
Game* game = NULL;
const int WINDOW_WIDTH = 1680;
const int WINDOW_HEIGHT = 900;

int main(int argc, char* argv[]) {

    //Game initialisation
    game = new Game();
    game->init("Game Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);

    //Game running loop
    while (game->isRunning()) {

        game->handleEvents();
        game->update();
        game->render();

    }

    //Game cleanup
    game->clean();

    return 0;
}
