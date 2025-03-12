#include "Game.h"
#include "Screens.h"

namespace myGame {
	Game::Game(int screenWidth, int screenHeight) :
		SCREEN_WIDTH{ screenWidth },
		SCREEN_HEIGHT{ screenHeight },
		GAME_WIDTH{ 1600 < screenWidth ? 1600 : screenWidth },
		GAME_HEIGHT{ 900 < screenHeight ? 900 : screenHeight },
		CAMERA_WIDTH{ 800 },
		CAMERA_HEIGHT{ 480 },
		screen{ nullptr },
		_quit{ false },
		camera(0, 0, 800, 480),
		data{}
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			_quit = true;
		}
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			_quit = true;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			_quit = true;
		}
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			_quit = true;
		}
		//Initialize SDL_ttf
		if (TTF_Init() == -1)
		{
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			_quit = true;
		}
		font = TTF_OpenFont(fontPath.c_str(), 28);
		if (font == NULL)
		{
			printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
			_quit = true;
		}
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, "***LOADING***", { 0,0,0 });
		if (textSurface == NULL)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			_quit = true;
		}
		//Create texture from surface pixels
		loadingTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (loadingTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			_quit = true;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);

		XMLDocument doc;
		doc.LoadFile("data.xml");
		if (!data.set(doc)) {
			printf("Error to load data! Error: %s\n", getError().c_str());
			_quit = true;
		}

		background = loadTexture(renderer, data.backgroundPath);
		if (background == NULL) {
			printf("Background load failed! Error: %s\n", getError().c_str());
			_quit = true;
		}

		
	}
	void Game::loadMainMenu()
	{
		screen = new MainMenu(this, &data, renderer);
	}
	void Game::setNewScreen(Screen* scr)
	{
		if(screen != nullptr) delete screen;
		screen = scr;
	}
	void Game::act(Uint32 delay)
	{
		camera.move(screen->getFocus(), screen->getRect(), delay);
		screen->act(delay);
	}
	void Game::handleEvent(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN)
		{
			//Adjust the velocity
			switch (e.key.keysym.sym)
			{
			case SDLK_UP: camera.move(Direction::Up, screen->getRect()); break;
			case SDLK_DOWN: camera.move(Direction::Down, screen->getRect()); break;
			case SDLK_LEFT: camera.move(Direction::Left, screen->getRect()); break;
			case SDLK_RIGHT: camera.move(Direction::Right, screen->getRect()); break;
			}
		}
	}
	void Game::render() {
		SDL_Texture* rawView = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, camera.rect.w, camera.rect.h);
		if (rawView == NULL)
		{
			printf("Unable to create streamable blank texture! SDL Error: %s\n", SDL_GetError());
		}
		SDL_SetRenderTarget(renderer, rawView);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		SDL_Rect back = { 0,0,camera.rect.w, camera.rect.h };
		SDL_RenderCopy(renderer, background, NULL, &back);

		screen->render(camera, renderer);

		SDL_Rect pos{
			0,
			0,
			GAME_WIDTH,
			camera.rect.h * (float)GAME_WIDTH / (float)camera.rect.w
		};
		//Clear screen
		SDL_SetRenderTarget(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, rawView, NULL, &pos);

		//Update screen
		SDL_RenderPresent(renderer);

		SDL_DestroyTexture(rawView);
	}
	int Game::quit() {
		delete screen;
		//Free global font
		TTF_CloseFont(font);
		font = NULL;
		SDL_DestroyTexture(loadingTexture);
		loadingTexture = NULL;
		SDL_DestroyTexture(background);
		background = NULL;

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();

		return 0;
	}
}