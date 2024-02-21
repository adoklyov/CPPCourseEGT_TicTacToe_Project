#include "Board.h"
#include <iostream>

//Constructor
Board::Board() {

	//Initial state of the board
	board.resize(3, vector<Position>(3, EMPTY));
	state = PLAY;
	win = false;
	draw = false;

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
    
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            win = true;
            return true; 
        }
        if (board[0][i] != EMPTY && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            win = true;
            return true;
        }
    }

    
    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        win = true;
        return true;
    }
    if (board[0][2] != EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        win = true;
        return true;
    }

    
    return false;
}