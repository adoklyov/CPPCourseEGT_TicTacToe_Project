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
	showRules = false;
	ready = false;
	madeMove = false;

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

	SDL_Color textColor = { 255, 255, 255 };

	//Button properties
	SDL_Texture* pressedButton = IMG_LoadTexture(gameRenderer, "assets/images/pressed.png");
	SDL_Texture* notPressedButton = IMG_LoadTexture(gameRenderer, "assets/images/notpressed.png");
	SDL_Texture* inactiveButton = IMG_LoadTexture(gameRenderer, "assets/images/inactive.png");

	startButton.setPressedStateTexture(pressedButton);
	startButton.setNotPressedStateTexture(notPressedButton);
	startButton.setInactiveStateTexture(inactiveButton);
	startButton.setButtonPosition(1300, 30, 200, 155);
	startButton.setText("Start", font, textColor, gameRenderer);

	infoButton.setPressedStateTexture(pressedButton);
	infoButton.setNotPressedStateTexture(notPressedButton);
	infoButton.setInactiveStateTexture(inactiveButton);
	infoButton.setButtonPosition(1300, 205, 200, 155);
	infoButton.setText("Info", font, textColor, gameRenderer);

	readyButton.setPressedStateTexture(pressedButton);
	readyButton.setNotPressedStateTexture(notPressedButton);
	readyButton.setInactiveStateTexture(inactiveButton);
	readyButton.setButtonPosition(1300, 380, 200, 155);
	readyButton.setText("Ready", font, textColor, gameRenderer);

	undoButton.setPressedStateTexture(pressedButton);
	undoButton.setNotPressedStateTexture(notPressedButton);
	undoButton.setInactiveStateTexture(inactiveButton);
	undoButton.setButtonPosition(1300, 555, 200, 155);
	undoButton.setText("Undo", font, textColor, gameRenderer);

	restartButton.setPressedStateTexture(pressedButton);
	restartButton.setNotPressedStateTexture(notPressedButton);
	restartButton.setInactiveStateTexture(inactiveButton);
	restartButton.setButtonPosition(1300, 730, 200, 155);
	restartButton.setText("Restart", font, textColor, gameRenderer);

	//Mixer properties
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* backgroundMusic = Mix_LoadMUS("assets/bgm.mp3");
	Mix_PlayMusic(backgroundMusic, -1);

	//Start button sound
	startButtonSound = Mix_LoadWAV("assets/start.mp3");

	//Info button sound
	infoButtonSound = Mix_LoadWAV("assets/info.mp3");

	//Load info image
	infoImage = IMG_LoadTexture(gameRenderer, "assets/images/info.png");

	running = true;
	return true;
}
	

//Render the game objects
void Game::render() {

	//Black background
	SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gameRenderer);

	startButton.render(gameRenderer);
	infoButton.render(gameRenderer);
	readyButton.render(gameRenderer);
	undoButton.render(gameRenderer);
	restartButton.render(gameRenderer);

	//Initial state rendering
	if (gameState == START) {
		startButton.setActive(true);
		readyButton.setActive(false);
		undoButton.setActive(false);
		restartButton.setActive(true);
		string welcomeMessage = "Press Start to begin and make your turn!";
		SDL_Color textColor = { 255, 255, 255, 255 };

		SDL_Surface* surfaceWelcomeMessage = TTF_RenderText_Solid(font, welcomeMessage.c_str(), textColor);
		SDL_Texture* textureWelcomeMessage = SDL_CreateTextureFromSurface(gameRenderer, surfaceWelcomeMessage);
		int welcomeMessageWidth = surfaceWelcomeMessage->w;
		int welcomeMessageHeight = surfaceWelcomeMessage->h;

		SDL_Rect welcomeMessagePos = {
			(1680 - welcomeMessageWidth) / 2, 
			(900 - welcomeMessageHeight) / 2,
			welcomeMessageWidth,
			welcomeMessageHeight
		};

		SDL_RenderCopy(gameRenderer, textureWelcomeMessage, NULL, &welcomeMessagePos);

	}

	//Over state
	if (gameState == OVER) {
		int cellSize = 300;
		int boardOffsetX = 0;
		int boardOffsetY = 0;
		
		//Win conditions when the game is over
		if (gameBoard->winHor1()) {

			int startY = boardOffsetY + cellSize / 2; 
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, boardOffsetX, startY, boardOffsetX + cellSize * 3, startY);
			winMessage();
		}

		if (gameBoard->winHor2()) {

			int startY = boardOffsetY + cellSize * 1.5; 
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, boardOffsetX, startY, boardOffsetX + cellSize * 3, startY);
			winMessage();
		}

		if (gameBoard->winHor3()) {
			int startY = boardOffsetY + cellSize * 2.5;
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, boardOffsetX, startY, boardOffsetX + cellSize * 3, startY);
			winMessage();
		}

		if (gameBoard->winVer1()) {
			int startX = boardOffsetX + cellSize / 2;
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, startX, boardOffsetY, startX, boardOffsetY + cellSize * 3);
			winMessage();
		}

		if (gameBoard->winVer2()) {
			int startX = boardOffsetX + cellSize * 1.5;
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, startX, boardOffsetY, startX, boardOffsetY + cellSize * 3);
			winMessage();
		}

		if (gameBoard->winVer3()) {
			int startX = boardOffsetX + cellSize * 2.5;
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, startX, boardOffsetY, startX, boardOffsetY + cellSize * 3);
			winMessage();
		}

		if (gameBoard->winDia1()) {
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, boardOffsetX + cellSize / 2, boardOffsetY + cellSize / 2, boardOffsetX + cellSize * 2.5, boardOffsetY + cellSize * 2.5);
			winMessage();
		}

		if (gameBoard->winDia2()) {
			SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(gameRenderer, boardOffsetX + cellSize * 2.5, boardOffsetY + cellSize / 2, boardOffsetX + cellSize / 2, boardOffsetY + cellSize * 2.5);
			winMessage();
		}

		
		startButton.setActive(true);
		readyButton.setActive(false);
		undoButton.setActive(false);
		restartButton.setActive(true);
		renderGame();
	}

	//Playing State rendering
	if (gameState == PLAYING) {
		startButton.setActive(false);
		restartButton.setActive(true);
		readyMessage();
		renderGame();
		currentPlayerTurn();
	}

	//Draw state rendering
	if (gameState == DRAW) {
		startButton.setActive(true);
		undoButton.setActive(false);
		readyButton.setActive(false);
		restartButton.setActive(true);
		drawMessage();
	}


	//Info button rules
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

			//Start Button
			if (startButton.isClicked(mouseX, mouseY) && gameState == START) {
				startButton.setPressed();
				Mix_PlayChannel(-1, startButtonSound, 0);
				reset();
				gameState = PLAYING;
				ready = true;
				startButton.setActive(true);
				undoButton.setActive(true);
				return;
			}

			if (startButton.isClicked(mouseX, mouseY) && gameState == OVER) {
				startButton.setPressed();
				Mix_PlayChannel(-1, startButtonSound, 0);
				reset();
				ready = true;
				gameState = PLAYING;

				return;
			}

			if (startButton.isClicked(mouseX, mouseY) && gameState == DRAW) {
				startButton.setPressed();
				Mix_PlayChannel(-1, startButtonSound, 0);
				reset();
				ready = true;
				gameState = PLAYING;
				return;
			}


			//Info Button
			if (infoButton.isClicked(mouseX, mouseY)) {
				Mix_PlayChannel(-1, infoButtonSound, 0);
				infoButton.setPressed();
				showRules = !showRules;
				return;
			}

			//Undo Button
			if (undoButton.isClicked(mouseX, mouseY) && madeMove && gameState == PLAYING) {
				undoButton.setPressed();
				gameBoard->undoMove();
				madeMove = false;
				undoButton.setActive(false);
				togglePlayerTurn();
				return;
			}

			//Ready Button
			if (readyButton.isClicked(mouseX, mouseY)) {
				readyButton.setPressed();
				ready = true;
				readyButton.setActive(false);
				undoButton.setActive(true);
			}

			//Restart Button
			if (restartButton.isClicked(mouseX, mouseY)) {
				restartButton.setPressed();
				reset();
				gameState == PLAYING;
				ready = true;
			}

			//Board clicks
			if (checkBoardClick(mouseX, mouseY)) {
				processBoardClick(mouseX, mouseY);
			}
		}
	}
}


//Update the game state and objects
void Game::update() {

	startButton.delayPress();
	readyButton.delayPress();
	undoButton.delayPress();
	infoButton.delayPress();
	
}





//Cleans up the game
void Game::clean() {

	//Clean up the window and renderer
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyTexture(infoImage);
	SDL_Quit();
	Mix_Quit();

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

	string winMessage = "Player " + string((playerTurn == oPlayer) ? " O " : " X ") + " wins the game!";
	SDL_Color textColor = { 255, 255, 255, 255 };

	SDL_Surface* surfaceWinMessage = TTF_RenderText_Solid(font, winMessage.c_str(), textColor);
	SDL_Texture* textureWinMessage = SDL_CreateTextureFromSurface(gameRenderer, surfaceWinMessage);
	int winMessageWidth = surfaceWinMessage->w;
	int winMessageHeight = surfaceWinMessage->h;

	SDL_Rect winMessagePos = { 1000, 250, winMessageWidth, winMessageHeight };
	SDL_RenderCopy(gameRenderer, textureWinMessage, NULL, &winMessagePos);

	string newGameMessage = "Press Start to begin a new game!";

	SDL_Surface* surfaceNewGameMessage = TTF_RenderText_Solid(font, newGameMessage.c_str(), textColor);
	SDL_Texture* textureNewGameMessage = SDL_CreateTextureFromSurface(gameRenderer, surfaceNewGameMessage);
	int newGameMessageWidth = surfaceNewGameMessage->w;
	int newGameMessageHeight = surfaceNewGameMessage->h;

	SDL_Rect newGameMessagePos = { 1000, 250 + winMessageHeight + 20, newGameMessageWidth, newGameMessageHeight };
	SDL_RenderCopy(gameRenderer, textureNewGameMessage, NULL, &newGameMessagePos);

}

//Message on ready press
void Game::readyMessage() {

	string readyMessage = "Press ready, before next turn.";

	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Surface* surfaceReadyMessage = TTF_RenderText_Solid(font, readyMessage.c_str(), textColor);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gameRenderer, surfaceReadyMessage);
	int textWidth = surfaceReadyMessage->w;
	int textHeight = surfaceReadyMessage->h;
	SDL_Rect MessagePos;
	MessagePos.x = 1000;
	MessagePos.y = 250;
	MessagePos.w = textWidth;
	MessagePos.h = textHeight;

	SDL_RenderCopy(gameRenderer, Message, NULL, &MessagePos);
}

//Message on draw state
void Game::drawMessage() {
	string drawMessage = "PARITY! Press Start for a new game and make your turn!";
	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, drawMessage.c_str(), textColor);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(gameRenderer, surfaceMessage);

	int textWidth = surfaceMessage->w;
	int textHeight = surfaceMessage->h;
	SDL_Rect MessagePos;
	MessagePos.x = 600;
	MessagePos.y = 250;
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
	readyButton.setActive(true); 
	undoButton.setActive(false);
	ready = false; 
	madeMove = false;
	showRules = false;
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
				//Draw Blue Circle
				SDL_SetRenderDrawColor(gameRenderer, 0, 0, 255, 255);
				int radius = cellSize / 4;
				int x = radius - 1;
				int y = 0;
				int tx = 1;
				int ty = 1;
				int err = tx - (radius << 1);

				while (x >= y) {
					SDL_RenderDrawPoint(gameRenderer, centerX + x, centerY - y);
					SDL_RenderDrawPoint(gameRenderer, centerX + x, centerY + y);
					SDL_RenderDrawPoint(gameRenderer, centerX - x, centerY - y);
					SDL_RenderDrawPoint(gameRenderer, centerX - x, centerY + y);
					SDL_RenderDrawPoint(gameRenderer, centerX + y, centerY - x);
					SDL_RenderDrawPoint(gameRenderer, centerX + y, centerY + x);
					SDL_RenderDrawPoint(gameRenderer, centerX - y, centerY - x);
					SDL_RenderDrawPoint(gameRenderer, centerX - y, centerY + x);

					if (err <= 0) {
						y++;
						err += ty;
						ty += 2;
					}

					if (err > 0) {
						x--;
						tx += 2;
						err += tx - (radius << 1);
					}
				}
			}
		}
	}
}

//Board clicking
void Game::processBoardClick(int mouseX, int mouseY) {
	int col = mouseX / 300;
	int row = mouseY / 300;

	if (ready) {
		PlayerState currentPlayerState = playerTurn->getPlayerState();
		Position pos = (currentPlayerState == XPLAYER) ? X : O;

		if (gameBoard->makeTurn(row, col, pos)) {
			madeMove = true;
			Mix_PlayChannel(-1, (pos == X) ? startButtonSound : infoButtonSound, 0);

			if (checkWinCon()) {
				gameState = OVER;
			}
			else if (gameBoard->drawCondition()) {
				gameState = DRAW;
			}
			else {
				togglePlayerTurn();
				readyButton.setActive(true);
			}
			ready = false;
		}
	}
}

//Checking win conditions
bool Game::checkWinCon() {
	if (gameBoard->winHor1() || gameBoard->winHor2() || gameBoard->winHor3() ||
		gameBoard->winVer1() || gameBoard->winVer2() || gameBoard->winVer3() ||
		gameBoard->winDia1() || gameBoard->winDia2()) {
		return true;
	}
	return false;
}

//Checking board click based on mouse
bool Game::checkBoardClick(int mouseX, int mouseY) {

	int boardX = 0; 
	int boardY = 0;
	int boardWidth = 3 * 300; 
	int boardHeight = 3 * 300;

	return (mouseX >= boardX && mouseX < boardX + boardWidth &&
		mouseY >= boardY && mouseY < boardY + boardHeight);
}

//Swaps player's turn
void Game::togglePlayerTurn() {

	if (playerTurn == oPlayer) {
		playerTurn = xPlayer;
	}
	else {
		playerTurn = oPlayer;
	}

	ready = false; 
	readyButton.setActive(true); 
}

//Message for current player turn
void Game::currentPlayerTurn() {

	SDL_Color textColor = { 255, 255, 255, 255 };

	string turnMessage = "Player " + string((playerTurn == xPlayer) ? "X" : "O") + "'s Turn";
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, turnMessage.c_str(), textColor);
	SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(gameRenderer, surfaceMessage);

	int messageWidth = surfaceMessage->w;
	int messageHeight = surfaceMessage->h;
	SDL_Rect messageRect = { (1680 - messageWidth) / 2, 20, messageWidth, messageHeight };

	SDL_RenderCopy(gameRenderer, messageTexture, NULL, &messageRect);

	SDL_DestroyTexture(messageTexture);
	SDL_FreeSurface(surfaceMessage); 
}