#pragma once
#include "Drawable.h"
#include "Entity.h"
#include <filesystem>
#include <iostream>
#include <SDL_Image.h>
#include <string>
#include <vector>

namespace fs = std::filesystem;

using std::vector, std::string;

namespace myGame {
	class Game;

	/* abstract */
	class Screen : public Drawable {
	protected:
		Game *game;
		Data* data;
		SDL_Renderer* renderer;
		Screen(Game* _game, Data* data, SDL_Renderer* r) : game(_game), data(data), renderer(r) {}
	};
}