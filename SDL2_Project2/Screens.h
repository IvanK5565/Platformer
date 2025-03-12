#pragma once
#include "Drawable.h"
#include "Entity.h"
#include "Tools.h"
#include <filesystem>
#include <iostream>
#include <SDL_Image.h>
#include <string>
#include <vector>

namespace fs = std::filesystem;

using std::vector, std::string;

namespace myGame {
	class Game;

	class Screen : public Drawable {
	protected:
		Game *game;
		Data* data;
		SDL_Renderer* renderer;
		Screen(Game* _game, Data* data, SDL_Renderer* r) : game(_game), data(data), renderer(r) {}
	};

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

	class Map : public Screen {
		int curLvl;
		int width, height;
		int** tileNums;
		int enemyCount;
		vector<Collide*> walls;
		vector<SDL_Texture*> tiles;
		vector<Entity*> enemies;
		Player* player;
		Rectangle winZone;
		SDL_Texture* deathScreen;
		SDL_Texture* winScreen;
		SDL_Texture* endGameTexture;
		bool endgame;
		Uint16 alfa;
	public:
		string error;
		bool isError;
		Map(Game*, Data*, SDL_Renderer*);

		virtual ~Map();
		virtual void render(Camera&, SDL_Renderer*) override;
		virtual void act(Uint32) override;
		virtual SDL_Rect getRect() override;
		virtual SDL_Point getFocus() override;
	private:
		bool loadTiles(XMLElement*);
		bool loadObjects(XMLElement*, Data*, SDL_Renderer*);
		bool loadTextures(SDL_Renderer*, Data*);
		bool loadMapData();
		bool nextLevel();
		void restart();
	};
}