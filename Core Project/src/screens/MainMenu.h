#pragma once
#include "Drawable.h"
#include "Entity.h"
#include "Screen.h"
#include <SDL_Image.h>
#include <string>
#include <vector>

namespace fs = std::filesystem;

using std::vector, std::string;

namespace myGame {
	class Game;
  
	class MainMenu : public Screen {
		SDL_Texture* playButtonTexture;
		SDL_Texture* exitButtonTexture;
		SDL_Rect play;
		SDL_Rect exit;

		int mX, mY;
		bool mPressed;
		
	public:
		MainMenu(Game* game, Data* data, SDL_Renderer* r);
		virtual ~MainMenu() {}
		virtual void render(Camera&, SDL_Renderer*) override;
		virtual void act(Uint32) override;
		virtual SDL_Rect getRect() override;
		virtual SDL_Point getFocus() override;
	};
}