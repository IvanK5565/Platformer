#define SDL_MAIN_HANDLED
#include "Game.h"
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

using namespace myGame;

int main(int argc, const int** args) {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	Game game(width, height);
	if (game._quit)
		return game.quit();

	bool quit = false;
	game.loadMainMenu();


	Uint32 oldTicks = 0;
	Uint32 delay = 0;

	while (!quit) {
	SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			game.handleEvent(e);
		}

		Uint32 newTicks = SDL_GetTicks();
		delay += newTicks - oldTicks;
		if (delay >= 10) {
			game.render();
			game.act(delay%100);
			delay = delay % 10;
		}
		oldTicks = newTicks;
	}
	return game.quit();
}
