#include "MainMenu.h"
#include "Map.h"
#include "Game.h"
#include "../common/TextureLoader.h"

using tinyxml2::XML_NO_ATTRIBUTE;
namespace myGame {
	MainMenu::MainMenu(Game* game, Data* data, SDL_Renderer* r)
		: Screen(game, data, r), play{ 0,0,0,0 }, exit{ 0,0,0,0 } {
		playButtonTexture = loadTexture(renderer, data->menu.playButPath);
		exitButtonTexture = loadTexture(renderer, data->menu.exitButPath);

		play.w = data->menu.width * 10;
		exit.w = data->menu.width * 10;
		play.h = data->menu.height * 10;
		exit.h = data->menu.height * 10;

		play.x = game->CAMERA_WIDTH / 2 - play.w / 2;
		play.y = game->CAMERA_HEIGHT / 2 - play.h / 2 - 100;
		exit.x = game->CAMERA_WIDTH / 2 - exit.w / 2;
		exit.y = game->CAMERA_HEIGHT / 2 - exit.h / 2 + 100;
	}
	void MainMenu::render(Camera& camera, SDL_Renderer* renderer) {

		SDL_RenderCopy(renderer, playButtonTexture, NULL, &play);
		SDL_RenderCopy(renderer, exitButtonTexture, NULL, &exit);
	}
	void MainMenu::act(Uint32 delay)
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		int mouseX = -1, mouseY = -1;

		if (SDL_GetMouseState(&mouseX, &mouseY) & 1) {
			mX = mouseX * game->CAMERA_WIDTH / game->GAME_WIDTH;
			mY = mouseY * game->CAMERA_HEIGHT / game->GAME_HEIGHT;
			mPressed = true;
			
		}
		else if (mPressed) {
			SDL_Point point{mX, mY};
			if (SDL_PointInRect(&point, &play)) {
				mPressed = false;
				game->setNewScreen(new Map(game, data, renderer));
				return;
			}
			if (SDL_PointInRect(&point, &exit)) {
				mPressed = false;
				SDL_Event event{};
				event.type = SDL_QUIT;
				event.quit.timestamp = SDL_GetTicks();
				SDL_PushEvent(&event);
				return;
			}
		}
	}
	SDL_Rect MainMenu::getRect()
	{
		return { 0,0,0,0 };
	}

	SDL_Point MainMenu::getFocus()
	{
		return SDL_Point();
	}
}