#ifndef BOARD_H
#define BOARD_H
#include <vector>

#include <SDL.h>

using namespace std;

enum Position { EMPTY , X , O };

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

	//Undo last turn
	void undoMove();

	//Draw condition
	bool drawCondition() const;

	//Win conditions
	bool winHor1();
	bool winHor2();
	bool winHor3();
	bool winVer1();
	bool winVer2();
	bool winVer3();
	bool winDia1();
	bool winDia2();

private:

	//Vector to store the state of the board
	vector<vector<Position>> board;

	//Attributes
	bool win;
	bool draw;
	int lastMoveRow = -1;
	int lastMoveCol = -1;
	Position lastMovePlayer = EMPTY;

};

#endif