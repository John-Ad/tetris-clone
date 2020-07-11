#include "game_engine.h"

int main()
{
	srand(time(NULL));

	game_engine game;
	
	while (game.running())
	{
		game.update();
		game.render();
	}
	return 0;
}