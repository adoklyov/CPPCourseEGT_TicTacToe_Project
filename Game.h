#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

#include "Player.h"
#include "Board.h"
#include "Button.h"

using namespace std;

enum GameState
{
    START,
    PAUSE,
    OVER,
    QUIT,
};

class Game {

public:

    //Constructor and destructor
    Game();
    ~Game();

    //Initialise the game window, renderer and game state
    bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
    //Render the game objects
    void render();
    //Update the game state and objects
    void update();
    //Processes events
    void handleEvents();
    //Cleans up the game
    void clean();
    //Checks if the game is running
    bool isRunning();
    //Changes the game state
    void changeState(GameState newState);

private:

    //SDL window and renderer
    SDL_Window* gameWindow = NULL;
    SDL_Renderer* gameRenderer = NULL;
    //Font
    TTF_Font* font;
    //Game running state
    bool running;
    //Players
    Player* xPlayer;
    Player* oPlayer;
    Player* playerTurn;
    //Board
    Board* gameBoard;
    //Buttons
    Button startButton;
    Button infoButton;
    Button readyButton;
    Button undoButton;
    //Game states
    GameState gameState;

};

#endif