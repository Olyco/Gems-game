#include "Game.h"

int main(void){

	Game game;
	int countOfClicks = 0;
	sf::Vector2i prevClickCoordinates;

	while (game.window.isOpen()) {
		
		game.Run(countOfClicks, prevClickCoordinates);
		game.DrawInWindow();
	}

	return 0;
}