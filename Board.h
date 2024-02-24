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

	//Undo last turn
	void undoMove();

	//Win conditions
	bool winCondition();

	//Win line draw
	void getWinLine(int& startRow, int& startCol, int& endRow, int& endCol) const;

	//Getters
	void getWinLineStart(int& row, int& col) const {
		row = winLineStartRow;
		col = winLineStartCol;
	}
	void getWinLineEnd(int& row, int& col) const {
		row = winLineEndRow;
		col = winLineEndCol;
	}

private:

	//Vector to store the state of the board
	vector<vector<Position>> board;
	State state;
	
	//Win Conditions
	int winLineStartRow, winLineStartCol, winLineEndRow, winLineEndCol;

	//Attributes
	bool win;
	bool draw;
	int lastMoveRow = -1;
	int lastMoveCol = -1;
	Position lastMovePlayer = EMPTY;

};

#endif