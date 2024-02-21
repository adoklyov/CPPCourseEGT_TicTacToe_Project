#include "Button.h"

//Constructor
Button::Button() {

	buttState = NOTPRESSED;
	pressedState = nullptr;
	notPressedState = nullptr;
	inactiveState = nullptr;
	butt = { 0, 0 , 100, 50 };

}

//Destructor
Button::~Button() {
	
	SDL_DestroyTexture(pressedState);
	SDL_DestroyTexture(notPressedState);
	SDL_DestroyTexture(inactiveState);

}

//Method to set the text of the button
void Button::setText(string text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {

	
	if (textTexture != nullptr) {
		SDL_DestroyTexture(textTexture);
		textTexture = nullptr; 
	}

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface != nullptr) { 
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_FreeSurface(textSurface);
	}
}

//Method to render the button based on the state
void Button::render(SDL_Renderer* renderer) {
	
	SDL_Texture* currentTexture = (buttState == PRESSED) ? pressedState : (buttState == NOTPRESSED) ? notPressedState : inactiveState;
	SDL_RenderCopy(renderer, currentTexture, NULL, &butt);

	if (textTexture != nullptr) {
		int textWidth, textHeight;
		SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
		SDL_Rect textRect = { butt.x + (butt.w - textWidth) / 2, butt.y + (butt.h - textHeight) / 2, textWidth, textHeight };
		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	}
}

//Method to handle the button position
void Button::setButtonPosition(int x, int y, int w, int h) {
	butt.x = x;
	butt.y = y;
	butt.w = w;
	butt.h = h;
}

//Method for state textures
void Button::setPressedStateTexture(SDL_Texture* texture) {
	pressedState = texture;
}

void Button::setNotPressedStateTexture(SDL_Texture* texture) {
	notPressedState = texture;
}

void Button::setInactiveStateTexture(SDL_Texture* texture) {
	inactiveState = texture;
}
