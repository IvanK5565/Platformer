#pragma once
#include "Camera.h"
#include "Data.h"
#include "Tools.h"
#include <SDL_ttf.h>
#include <windows.h>
#include <string>

using std::string;

namespace myGame {

	class Screen;

	class Game {
	private:
		const string fontPath = "./assets/fonts/monogram.ttf";
		SDL_Window* window;
		SDL_Renderer* renderer;
		TTF_Font* font;
		SDL_Texture* loadingTexture;
		SDL_Texture* background;
		Camera camera;
		Screen* screen;
		Data data;
		Game();
	public:
		const int SCREEN_WIDTH, SCREEN_HEIGHT;
		const int GAME_WIDTH, GAME_HEIGHT;
		const int CAMERA_WIDTH, CAMERA_HEIGHT;
		bool _quit;
		Game(int, int);
		void loadMainMenu();
		void setNewScreen(Screen* scr);
		void act(Uint32 delay);
		void handleEvent(SDL_Event& e);
		void render();
		int quit();
	};
}