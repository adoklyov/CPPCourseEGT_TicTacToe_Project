#include "Game.h"
#include "Player.h"
#include "Board.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

//Constructor
Game::Game() {

	Game::gameWindow = NULL;
	Game::gameRenderer = NULL;
	Game::running = true;
	oPlayer = new Player("Player O", OPLAYER);
	xPlayer = new Player("Player X", XPLAYER);
	playerTurn = xPlayer;
	gameBoard = new Board();
	gameState = START;
	startButton.setActive(true);
	winDelayTimer = 0;
	winDisplay = false;
	showRules = false;

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
	startButton.setButtonPosition(1240, 100, 230, 155);
	startButton.setText("Start", font, textColor, gameRenderer);

	infoButton.setPressedStateTexture(pressedButton);
	infoButton.setNotPressedStateTexture(notPressedButton);
	infoButton.setInactiveStateTexture(inactiveButton);
	infoButton.setButtonPosition(1240, 275, 230, 155);
	infoButton.setText("Info", font, textColor, gameRenderer);

	readyButton.setPressedStateTexture(pressedButton);
	readyButton.setNotPressedStateTexture(notPressedButton);
	readyButton.setInactiveStateTexture(inactiveButton);
	readyButton.setButtonPosition(1240, 450, 230, 155);
	readyButton.setText("Ready", font, textColor, gameRenderer);

	undoButton.setPressedStateTexture(pressedButton);
	undoButton.setNotPressedStateTexture(notPressedButton);
	undoButton.setInactiveStateTexture(inactiveButton);
	undoButton.setButtonPosition(1240, 625, 230, 155);
	undoButton.setText("Undo", font, textColor, gameRenderer);

	//Mixer properties
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* backgroundMusic = Mix_LoadMUS("assets/bgm.mp3");
	Mix_PlayMusic(backgroundMusic, -1);

	//Load win line image
	SDL_Surface* tmpSurface = IMG_Load("assets/images/line.png");
	winImage = SDL_CreateTextureFromSurface(gameRenderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	//Load info image
	infoImage = IMG_LoadTexture(gameRenderer, "assets/images/info.png");
	if (!infoImage) {
		cout << "Failed to load rules image: " << IMG_GetError() << endl;
	}

	running = true;
	return true;
}
	

//Render the game objects
void Game::render() {

	int cellSize = 300;

	//Black background
	SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gameRenderer);

	//Initial state rendering
	if (gameState == START || gameState == OVER) {
		startButton.setActive(true);
		startButton.render(gameRenderer);
		infoButton.render(gameRenderer);
		readyButton.setActive(false);
		readyButton.render(gameRenderer);
		undoButton.render(gameRenderer);
		undoButton.setActive(false);
	}

	//Over state rendering
	if (gameState == OVER) {
		winMessage();
		startButton.setActive(true);
		startButton.render(gameRenderer);
		undoButton.render(gameRenderer);
	}

	//Playing State rendering
	if (gameState == PLAYING) {
		startButton.setActive(false);
		startButton.render(gameRenderer);
		infoButton.render(gameRenderer);
		readyButton.render(gameRenderer);
		readyButton.setActive(true);
		undoButton.render(gameRenderer);
		undoButton.setActive(true);
		renderGame();

	}

	if (winDisplay) {
		int cellSize = 350;
		int startRow, startCol, endRow, endCol;
		//Retrieve the win line start and end coordinates from the Board object
		board.getWinLineStart(startRow, startCol);
		board.getWinLineEnd(endRow, endCol);

		//Convert to pixel coordinates
		int startX = startCol * cellSize + cellSize * 4;
		int startY = startRow * cellSize + cellSize * 2;
		int endX = endCol * cellSize + cellSize;
		int endY = endRow * cellSize + cellSize;

		//Log the attempt to draw the line
		//cout << "Attempting to draw win line: Start (" << startX << ", " << startY << ") to End (" << endX << ", " << endY << ")" << endl;

		//Set draw color to green for the win line
		SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(gameRenderer, startX, startY, endX, endY);
	}

	/*if (winDisplay) {
		//Hardcoded all win lines on win
		vector<pair<pair<int, int>, pair<int, int>>> winLines = {
			//Horizontal wins
			{{0 * cellSize + cellSize / 2, 0 * cellSize + cellSize / 2}, {2 * cellSize + cellSize / 2, 0 * cellSize + cellSize / 2}}, //Top row
			{{0 * cellSize + cellSize / 2, 1 * cellSize + cellSize / 2}, {2 * cellSize + cellSize / 2, 1 * cellSize + cellSize / 2}}, //Middle row
			{{0 * cellSize + cellSize / 2, 2 * cellSize + cellSize / 2}, {2 * cellSize + cellSize / 2, 2 * cellSize + cellSize / 2}}, //Bottom row

			//Vertical wins
			{{0 * cellSize + cellSize / 2, 0 * cellSize + cellSize / 2}, {0 * cellSize + cellSize / 2, 2 * cellSize + cellSize / 2}}, //Left column
			{{1 * cellSize + cellSize / 2, 0 * cellSize + cellSize / 2}, {1 * cellSize + cellSize / 2, 2 * cellSize + cellSize / 2}}, //Middle column
			{{2 * cellSize + cellSize / 2, 0 * cellSize + cellSize / 2}, {2 * cellSize + cellSize / 2, 2 * cellSize + cellSize / 2}}, //Right column

			//Diagonal wins
			{{0 * cellSize + cellSize / 2, 0 * cellSize + cellSize / 2}, {2 * cellSize + cellSize / 2, 2 * cellSize + cellSize / 2}}, //Top-left to bottom-right
			{{2 * cellSize + cellSize / 2, 0 * cellSize + cellSize / 2}, {0 * cellSize + cellSize / 2, 2 * cellSize + cellSize / 2}}  //Top-right to bottom-left
		};

		for (const auto& line : winLines) {
			const auto& start = line.first;
			const auto& end = line.second;

			cout << "Attempting to draw win line: Start (" << start.first << ", " << start.second << ") to End (" << end.first << ", " << end.second << ")" << endl;

			//Green win line
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, start.first, start.second, end.first, end.second);
		}
	}*/

	if (showRules) {

		SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 128);
		SDL_Rect overlayRect = { 0, 0, 1680, 900 };
		SDL_RenderFillRect(gameRenderer, &overlayRect);

		SDL_Rect rulesRect = { 100, 100, 600, 400 };
		SDL_RenderCopy(gameRenderer, infoImage, NULL, &rulesRect);
	}

	SDL_RenderPresent(gameRenderer);

}


//Processes events
void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			
			if (startButton.isClicked(mouseX, mouseY)) {
				reset();
				gameState = PLAYING; 
				startButton.setActive(true);
				winDisplay = false;
				return; 
			}

			if (infoButton.isClicked(mouseX, mouseY)) {
				showRules = !showRules;
			}

			if (readyButton.isClicked(mouseX, mouseY) && gameState == PLAYING) {
				playerTurn = (playerTurn == xPlayer) ? oPlayer : xPlayer;
			}

			int boardX = 0; 
			int boardY = 0;
			int boardWidth = 3 * 300; 
			int boardHeight = 3 * 300;


			mouseX -= boardX;
			mouseY -= boardY;

			if (startButton.isClicked(mouseY, mouseY) && gameState == START) {
				gameState = PLAYING;
				startButton.setActive(false);
			}

			if (mouseX >= 0 && mouseX < boardWidth && mouseY >= 0 && mouseY < boardHeight) {
				int col = mouseX / 300;
				int row = mouseY / 300; 

				PlayerState currentPlayerState = playerTurn->getPlayerState();
				Position pos = (currentPlayerState == XPLAYER) ? X : O;

				if (gameBoard->makeTurn(row, col, pos)) {

					playerTurn = (playerTurn == xPlayer) ? oPlayer : xPlayer;

					if (gameBoard->winCondition()) {
						cout << "YOU WIN!" << endl;
						winDisplay = true;

					}
				}

				cout << "Clicked cell at [" << row << ", " << col << "]" << endl;
			}
		}
	}
}


//Update the game state and objects
void Game::update() {
	
	if (gameState == OVER) {
		startButton.setActive(true);
	}

	if (gameState == PLAYING && board.winCondition() && !winDisplay) {
		gameState = OVER;
		winDelayTimer = SDL_GetTicks(); 
		winDisplay = true;
	}

	if (gameState == PLAYING && board.winCondition() && !winDisplay) {
		winDisplay = true;
		winDelayTimer = SDL_GetTicks();
	}



	if (gameState == OVER && winDisplay && SDL_GetTicks() - winDelayTimer > 3000) { 
		winDisplay = false;
		reset(); 
		gameState = START; 
	}
}





//Cleans up the game
void Game::clean() {

	//Clean up the window and renderer
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyTexture(infoImage);
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
	if (newState == PLAYING) {
		startButton.setActive(false);
	}
	else if (newState == OVER) {
		startButton.setActive(true);
	}	
}

//Message on win state
void Game::winMessage() {
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

//Reset game
void Game::reset() {
	gameBoard->reset();
	playerTurn = oPlayer;
	gameState = PLAYING;
	startButton.setActive(true);
}

//Grid render
void Game::renderGame() {
	//White grid lines
	SDL_SetRenderDrawColor(gameRenderer, 255, 255, 255, 255);
	int cellSize = 300;

	//Draw the grid lines
	for (int i = 1; i <= 2; ++i) {
		SDL_RenderDrawLine(gameRenderer, i * cellSize, 0, i * cellSize, 3 * cellSize);
		SDL_RenderDrawLine(gameRenderer, 0, i * cellSize, 3 * cellSize, i * cellSize);
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
}