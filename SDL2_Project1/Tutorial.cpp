#define SDL_MAIN_HANDLED
#include "Collide.h"
#include <iostream>
#include <string>
#include <tinyxml.h>
#include <SDL.h>
#include <SDL_ttf.h>

SDL_Window* window;
SDL_Renderer* renderer;

const int W = 800, H = 400;
const int speed = 10;


int main(int argc, const int** args) {
	Square box(100,100,100,100);

	RightTriangle rTri(300, 300);
	LeftTriangle lTri(100, 300);
	
	bool quit = false;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		quit = true;
	}
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		quit = true;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		quit = true;
	}
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		quit = true;
	}
	TTF_Font* font = TTF_OpenFont("monogram.ttf", 36);
	if (font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		quit = true;
	}


	Uint32 oldTicks = 0;
	Uint32 delay = 0;

	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
		}


		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[SDL_SCANCODE_A])
		{
			box.x -= speed;
			if (box.x < 0) box.x = 0;
		}
		if (currentKeyStates[SDL_SCANCODE_D])
		{
			box.x += speed;
			if (box.x + box.width > W) box.x = W -box.width;
		}
		if (currentKeyStates[SDL_SCANCODE_W])
		{
			box.y -= speed;
			if (box.y < 0) box.y = 0;
		}
		if (currentKeyStates[SDL_SCANCODE_S])
		{
			box.y += speed;
			if (box.y + box.height > H) box.y = H -box.height;
		}

		if (CollisionInfo info = rTri.checkCollision(box)) {
			int _x = std::abs(info.x1) < std::abs(info.x2) ? info.x1 : info.x2;
			int _y = std::abs(info.y1) < std::abs(info.y2) ? info.y1 : info.y2;

			if (std::abs(_x) < std::abs(_y)) {
				box.x += _x;
			}
			else {
				box.y += _y;
			}
		}
		if (CollisionInfo info = lTri.checkCollision(box)) {
			int _x = std::abs(info.x1) < std::abs(info.x2) ? info.x1 : info.x2;
			int _y = std::abs(info.y1) < std::abs(info.y2) ? info.y1 : info.y2;

			if (std::abs(_x) < std::abs(_y)) {
				box.x += _x;
			}
			else {
				box.y += _y;
			}
		}

		//render
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		std::string text;
		text += std::to_string(box.x);
		text += ", ";
		text += std::to_string(box.y);

		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), {255,0,0});
		if (textSurface == NULL)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else
		{
			//Create texture from surface pixels
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
			if (texture == NULL)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				SDL_Rect pos = { 0,0,100,50 };
				SDL_RenderCopy(renderer, texture, NULL, &pos);
			}

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
			SDL_DestroyTexture(texture);
		}

		box.drawBorder(renderer);
		rTri.drawBorder(renderer);
		lTri.drawBorder(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(500);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);


	TTF_Quit();
	SDL_Quit();
	return 0;
}