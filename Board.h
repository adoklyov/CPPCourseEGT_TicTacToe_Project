#ifndef BOARD_H
#define BOARD_H
#include <vector>

#include <SDL.h>

using namespace std;

enum Position { EMPTY , X , O };
enum State { PLAY , DRAW , WIN };

class Board {

public:
	
	//Constructor
	Board();

	//Method to reset the game state
	void reset();

	//Checks the position
	Position checkPosition(int row, int col);

	//Make turn
	bool makeTurn(int row, int col, Position p);

	//Win conditions
	bool winCondition();

private:

	//Vector to store the state of the board
	vector<vector<Position>> board;
	State state;

	//Attributes
	bool win;
	bool draw;

};

#endif