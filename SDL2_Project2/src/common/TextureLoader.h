#pragma once
#include "ErrorLog.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <string>
#include <vector>
#include <queue>
using std::string, std::vector, std::queue;

namespace myGame {
	static SDL_Texture* loadTexture(SDL_Renderer* renderer, string path) {
		SDL_Texture* newTexture = NULL;
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) {
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
			setError("Unable to load image " + path + "!SDL_image Error : " + IMG_GetError() + "\n");
			return NULL;
		}

		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			setError("Unable to load image " + path + "!SDL_image Error : " + SDL_GetError() + "\n");
			return NULL;
		}
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
		return newTexture;
	}
}