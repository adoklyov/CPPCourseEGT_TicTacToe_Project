#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

using namespace std;

enum PlayerState { XPLAYER, OPLAYER };

class Player {

public:

	//Constructor
	Player(string name, PlayerState state);

	//Methods
	//Returns player name
	string getName();
	//Returns player state
	PlayerState getPlayerState();


private:

	//Attributes
	string name;
	PlayerState pState;


};

#endif 
