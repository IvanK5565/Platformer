#pragma once
#include "Enums.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
using std::string, std::vector, std::queue;

namespace myGame {
	static size_t split(const string& txt, vector<string>& strs, char ch)
	{
		size_t pos = txt.find(ch);
		size_t initialPos = 0;
		strs.clear();

		// Decompose statement
		while (pos != string::npos) {
			strs.push_back(txt.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}

		// Add the last one
		strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

		return strs.size();
	}
	static vector<string> split(const string& txt, char ch)
	{
		vector<string> strs;
		split(txt, strs, ch);
		return strs;
	}
	static StatusCode code;
	static queue<string> errors;
	static string getError() {
		if (errors.empty()) return "";
		string buf = errors.front();
		errors.pop();
		if (errors.empty()) code = StatusCode::CORRECT;
		return buf;
	}
	static void setError(string mess) {
		errors.push(mess);
		code = StatusCode::UNCORRECT;
	}

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