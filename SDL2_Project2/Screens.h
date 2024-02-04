#pragma once
#include "Drawable.h"
#include "Entity.h"
#include "Tools.h"
#include <filesystem>
#include <iostream>
#include <SDL_Image.h>
#include <string>
#include <tinyxml.h>
#include <vector>

namespace fs = std::filesystem;

using std::vector, std::string;

namespace myGame {
	class MainMenu : public Drawable {
	public:
		virtual ~MainMenu() {}
		virtual void handleEvent(SDL_Event& e) override;
		virtual void render(Camera&, SDL_Renderer*) override;
		virtual void act(Uint32) override;
		virtual SDL_Rect getRect() override;
		virtual SDL_Point getFocus() override;
	};

	class Map : public Drawable {
		int width, height;
		int** tileNums;
		vector<Collide*> collides;
		vector<SDL_Texture*> tiles;
		vector<Entity*> enemies;
		Player* player;
	public:
		string error;
		bool isError;
		Map(Data*, SDL_Renderer*);

		virtual ~Map() {
			for (size_t i = 0; i < height; i++)
			{
				delete[] tileNums[i];
			}
			delete[] tileNums;

			for (size_t i = 0; i < collides.size(); i++)
			{
				delete collides[i];
			}
			for (size_t i = 0; i < tiles.size(); i++)
			{
				SDL_DestroyTexture(tiles[i]);
			}

			delete player;
		}
		virtual void handleEvent(SDL_Event& e) override;
		virtual void render(Camera&, SDL_Renderer*) override;
		virtual void act(Uint32) override;
		virtual SDL_Rect getRect() override;
		virtual SDL_Point getFocus() override;
	private:
		bool loadTiles(TiXmlElement*);
		bool loadObjects(TiXmlElement*, Data*, SDL_Renderer*);
		bool loadTextures(SDL_Renderer*, Data*);
	};
}