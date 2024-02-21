#include "Player.h"

//Constructor
Player::Player(string name, PlayerState state) {
	this->name = name;
	this->pState = state;
}

//Method to get the name of the player
string Player::getName() {
	return name;
}

//Method to get the state of the player
PlayerState Player::getPlayerState() {
	return pState;
}