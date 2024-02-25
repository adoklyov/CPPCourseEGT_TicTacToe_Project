#include "Board.h"
#include <iostream>

//Constructor
Board::Board() {

	//Initial state of the board
	board.resize(3, vector<Position>(3, EMPTY));
	win = false;
	draw = false;

}

//Method to reset the game state
void Board::reset() {

	board.clear();
	board.resize(3, vector<Position>(3, EMPTY));

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
        lastMoveRow = row;
        lastMoveCol = col;
        lastMovePlayer = EMPTY;
		cout << "Test position: " << row << "," << col << "," << (pos == X ? "X" : "O") << endl;
		return true;
	}
	return false;
}

//Method to undo last move
void Board::undoMove() {
    if (lastMoveRow != -1 && lastMoveCol != -1) {
        board[lastMoveRow][lastMoveCol] = EMPTY;
        lastMoveRow = -1;
        lastMoveCol = -1;
        lastMovePlayer = EMPTY;
    }
}

//Win condition checks
//Top row win
bool Board::winHor1() {
    if (board[0][0] != EMPTY && board[0][0] == board[0][1] && board[0][1] == board[0][2]) {
        win = true;
        return true;
    }
    return false;
}

//Middle row win
bool Board::winHor2() {
    if (board[1][0] != EMPTY && board[1][0] == board[1][1] && board[1][1] == board[1][2]) {
        win = true;
        return true;
    }
    return false;
}

//Bottom row win
bool Board::winHor3() {
    if (board[2][0] != EMPTY && board[2][0] == board[2][1] && board[2][1] == board[2][2]) {
        win = true;
        return true;
    }
    return false;
}

//Left column win
bool Board::winVer1() {
    if (board[0][0] != EMPTY && board[0][0] == board[1][0] && board[1][0] == board[2][0]) {
        win = true;
        return true;
    }
    return false;
}

//Middle column win
bool Board::winVer2() {
    if (board[0][1] != EMPTY && board[0][1] == board[1][1] && board[1][1] == board[2][1]) {
        win = true;
        return true;
    }
    return false;
}

//Right column win
bool Board::winVer3() {
    if (board[0][2] != EMPTY && board[0][2] == board[1][2] && board[1][2] == board[2][2]) {
        win = true;
        return true;
    }
    return false;
}

//Left to right diagonal win
bool Board::winDia1() {
    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        win = true;
        return true;
    }
    return false;
}

//Right to left diagonal win
bool Board::winDia2() {
    if (board[0][2] != EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        win = true;
        return true;
    }
    return false;
}

//Method to check for draw condition
bool Board::drawCondition() const {
    for (const auto& row : board) {
        for (Position pos : row) {
            if (pos == EMPTY) {
                return false; 
            }
        }
    }
    return !win; 
}