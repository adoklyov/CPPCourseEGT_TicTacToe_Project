#include "Board.h"
#include <iostream>

//Constructor
Board::Board() {

	//Initial state of the board
	board.resize(3, vector<Position>(3, EMPTY));
	state = PLAY;
	win = false;
	draw = false;
    winLineStartRow = -1;
    winLineStartCol = -1;
    winLineEndRow = -1;
    winLineEndCol = -1;
}

//Method to reset the game state
void Board::reset() {

	board.clear();
	board.resize(3, vector<Position>(3, EMPTY));
	state = PLAY;

}

//Method to check the position
Position Board::checkPosition(int row, int col) {

	if (row >= 0 && row < 3 && col >= 0 && col < 3) {
		return board[row][col];
	}
	return EMPTY;
}

//Method to make a turn
bool Board::makeTurn(int row, int col, Position pos) {
	if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == EMPTY) {
		board[row][col] = pos;
		cout << "Test position: " << row << "," << col << "," << (pos == X ? "X" : "O") << endl;
		return true;
	}
	return false;
}

//Method to check the win condition
bool Board::winCondition() {
    //Reset win line coordinates at the start of each check
    winLineStartRow = winLineStartCol = winLineEndRow = winLineEndCol = -1;

    //Horizontal check
    for (int row = 0; row < 3; row++) {
        if (board[row][0] != EMPTY && board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            winLineStartRow = row;
            winLineStartCol = 0;
            winLineEndRow = row;
            winLineEndCol = 2;
            win = true;
            cout << "Horizontal win at row: " << row << endl;
            return true;
        }
    }

    //Vertical check
    for (int col = 0; col < 3; col++) {
        if (board[0][col] != EMPTY && board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            winLineStartRow = 0;
            winLineStartCol = col;
            winLineEndRow = 2;
            winLineEndCol = col;
            win = true;
            cout << "Vertical win at col: " << col << endl;
            return true;
        }
    }

    //Diagonal check (top-left to bottom-right)
    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        winLineStartRow = 0;
        winLineStartCol = 0;
        winLineEndRow = 2;
        winLineEndCol = 2;
        win = true;
        cout << "Diagonal win from top-left to bottom-right" << endl;
        return true;
    }

    //Diagonal check (top-right to bottom-left)
    if (board[0][2] != EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        winLineStartRow = 0;
        winLineStartCol = 2;
        winLineEndRow = 2;
        winLineEndCol = 0;
        win = true;
        std::cout << "Diagonal win from top-right to bottom-left" << std::endl;
        return true;
    }

    return false;
}


//Method to get the win line
void Board::getWinLine(int& startRow, int& startCol, int& endRow, int& endCol) const {
    startRow = winLineStartRow;
    startCol = winLineStartCol;
    endRow = winLineEndRow;
    endCol = winLineEndCol;
}