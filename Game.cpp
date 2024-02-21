#include "Game.h"
#include "Player.h"
#include "Board.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>

//Constructor
Game::Game() {

	Game::gameWindow = NULL;
	Game::gameRenderer = NULL;
	Game::running = true;
	oPlayer = new Player("Player O", OPLAYER);
	xPlayer = new Player("Player X", XPLAYER);
	playerTurn = oPlayer;
	gameBoard = new Board();

}

//Destructor
Game::~Game() {
	clean();
	delete gameBoard;
}


//Initialise the game window
bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	gameWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, 0);

	//Load font
	font = TTF_OpenFont("assets/swan.ttf", 19);
	if (!font) {
		cout << "Failed to load font." << endl;
		return false;
	}

	SDL_Color textColor = { 255, 255, 255 };

	//Button properties
	SDL_Texture* pressedButton = IMG_LoadTexture(gameRenderer, "assets/images/pressed.png");
	SDL_Texture* notPressedButton = IMG_LoadTexture(gameRenderer, "assets/images/notpressed.png");
	SDL_Texture* inactiveButton = IMG_LoadTexture(gameRenderer, "assets/images/inactive.png");

	startButton.setPressedStateTexture(pressedButton);
	startButton.setNotPressedStateTexture(notPressedButton);
	startButton.setInactiveStateTexture(inactiveButton);
	startButton.setButtonPosition(1240, 200, 230, 155);
	startButton.setText("Start", font, textColor, gameRenderer);

	infoButton.setPressedStateTexture(pressedButton);
	infoButton.setNotPressedStateTexture(notPressedButton);
	infoButton.setInactiveStateTexture(inactiveButton);
	infoButton.setButtonPosition(1240, 500, 230, 155);
	infoButton.setText("Info", font, textColor, gameRenderer);

	//Mixer properties
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* backgroundMusic = Mix_LoadMUS("assets/bgm.mp3");
	Mix_PlayMusic(backgroundMusic, -1);

	running = true;
	return true;
}
	

//Render the game objects
void Game::render() {

	//Black background
	SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gameRenderer);

	//White grid lines
	SDL_SetRenderDrawColor(gameRenderer, 255, 255, 255, 255);
	int cellSize = 300;

	//Draw the grid lines
	for (int i = 1; i <= 2; ++i) {
		SDL_RenderDrawLine(gameRenderer, i * cellSize, 0, i * cellSize, 3 * cellSize);
		SDL_RenderDrawLine(gameRenderer, 0, i * cellSize, 3 * cellSize, i * cellSize);
	}

	//Message on win state
	if (gameState == WIN) {
		string message = "Player " + string((playerTurn == xPlayer) ? "X" : "O") + "wins the game!";
		SDL_Color textColor = { 255 , 255 , 255 , 255 };
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), textColor);
		SDL_Texture* Message = SDL_CreateTextureFromSurface(gameRenderer, surfaceMessage);

		int textWidth = surfaceMessage->w;
		int textHeight = surfaceMessage->h;
		SDL_Rect MessagePos;
		MessagePos.x = 100;
		MessagePos.y = 100;
		MessagePos.w = textWidth;
		MessagePos.h = textHeight;

		SDL_RenderCopy(gameRenderer, Message, NULL, &MessagePos);
	}

	//Draw Xs and 0s
	for (int row = 0; row < 3; ++row) {
		for (int col = 0; col < 3; ++col) {
			Position pos = gameBoard->checkPosition(row, col);

			int centerX = col * cellSize + cellSize / 2;
			int centerY = row * cellSize + cellSize / 2;

			if (pos == X) {
				//Draw Red X
				SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
				int offset = cellSize / 4;
				SDL_RenderDrawLine(gameRenderer, centerX - offset, centerY - offset, centerX + offset, centerY + offset);
				SDL_RenderDrawLine(gameRenderer, centerX + offset, centerY - offset, centerX - offset, centerY + offset);
			}
			else if (pos == O) {
				//Draw Blue Rect
				SDL_SetRenderDrawColor(gameRenderer, 0, 0, 255, 255);
				int radius = cellSize / 4;
				SDL_Rect rect = { centerX - radius, centerY - radius, 2 * radius, 2 * radius };
				SDL_RenderDrawRect(gameRenderer, &rect);
			}
		}
	}



	//Render the buttons
	startButton.render(gameRenderer);
	infoButton.render(gameRenderer);

	SDL_RenderPresent(gameRenderer);
}


//Processes events
void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			int boardX = 0; 
			int boardY = 0;
			int boardWidth = 3 * 300; 
			int boardHeight = 3 * 300;


			mouseX -= boardX;
			mouseY -= boardY;

			if (mouseX >= 0 && mouseX < boardWidth && mouseY >= 0 && mouseY < boardHeight) {
				int col = mouseX / 300;
				int row = mouseY / 300; 

				PlayerState currentPlayerState = playerTurn->getPlayerState();
				Position pos = (currentPlayerState == XPLAYER) ? X : O;

				if (gameBoard->makeTurn(row, col, pos)) {

					playerTurn = (playerTurn == xPlayer) ? oPlayer : xPlayer;

					if (gameBoard->winCondition()) {
						cout << "YOU WIN!" << endl;

						gameState = GameState::OVER; 

					}
				}

				std::cout << "Clicked cell at [" << row << ", " << col << "]" << std::endl;
			}
		}
	}
}


//Update the game state and objects
void Game::update() {
	

}



//Cleans up the game
void Game::clean() {

	//Clean up the window and renderer
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(gameRenderer);
	SDL_Quit();

}

//Checks if the game is running
bool Game::isRunning() {

	return Game::running;

}

//Change the game state
void Game::changeState(GameState newState)
{
	gameState = newState;
}