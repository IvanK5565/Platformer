#define SDL_MAIN_HANDLED
#include "Game.h"
#include <tinyxml.h>
#include <SDL.h>
#include <SDL_thread.h>

using namespace myGame;

int main(int argc, const int** args) {
	//SDL_SetMainReady();
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	Game game(width, height);
	bool quit = game._quit;
	//game.loadMainMenu();
	game.loadTest();


	Uint32 oldTicks = 0;
	Uint32 delay = 0;

	while (!quit) {
	//Event handler
	SDL_Event e;
		//break;
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0) {
				Uint32 click = SDL_GetMouseState(NULL, NULL);

				std::cout << "mouse\n";
				if (click & 1) {
					std::cout << "leftButton\n";
				}
				if (click & 2) {
					std::cout << "middleButton\n";
				}
				if (click & 4) {
					std::cout << "rightButton\n";
				}
			}
			//Handle input
			game.handleEvent(e);
		}

		Uint32 newTicks = SDL_GetTicks();
		delay += newTicks - oldTicks;
		if (delay >= 10) {
			game.act(delay);
			delay = delay % 10;
			game.render();
		}
		oldTicks = newTicks;
	}
	return game.quit();
}