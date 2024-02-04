#pragma once
#include "Camera.h"
#include "Drawable.h"
#include "Screens.h"
#include <SDL_ttf.h>
#include <windows.h>

namespace myGame {
	class Game {
	private:
		const string levelFolder = "C:\\Users\\UserOne\\Desktop\\";
		const string fontPath = "C:\\Users\\UserOne\\Desktop\\monogram.ttf";
		SDL_Window* window;
		SDL_Renderer* renderer;
		TTF_Font* font;
		SDL_Texture* loadingTexture;
		SDL_Texture* background;
		Camera camera;
		Drawable* screen;
		Data data;
		Game();
	public:
		const int SCREEN_WIDTH, SCREEN_HEIGHT;
		const int GAME_WIDTH, GAME_HEIGHT;
		bool _quit;
		Game(int, int);
		bool loadMainMenu();
		void loadTest();
		void setNewScreen();
		void act(Uint32 delay);
		void handleEvent(SDL_Event& e);
		void render();
		int quit();
	};
}