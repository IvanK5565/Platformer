#include "Screens.h"
#include "Game.h"
using tinyxml2::XML_NO_ATTRIBUTE;
namespace myGame {
	MainMenu::MainMenu(Game* game, Data* data, SDL_Renderer* r)
		: Screen(game, data, r), play{ 0,0,0,0 }, exit{ 0,0,0,0 } {
		playButtonTexture = loadTexture(renderer, data->menu.playButPath);
		exitButtonTexture = loadTexture(renderer, data->menu.exitButPath);

		play.w = data->menu.width * 10;
		exit.w = data->menu.width * 10;
		play.h = data->menu.height * 10;
		exit.h = data->menu.height * 10;

		play.x = game->CAMERA_WIDTH / 2 - play.w / 2;
		play.y = game->CAMERA_HEIGHT / 2 - play.h / 2 - 100;
		exit.x = game->CAMERA_WIDTH / 2 - exit.w / 2;
		exit.y = game->CAMERA_HEIGHT / 2 - exit.h / 2 + 100;
	}
	void MainMenu::render(Camera& camera, SDL_Renderer* renderer) {

		SDL_RenderCopy(renderer, playButtonTexture, NULL, &play);
		SDL_RenderCopy(renderer, exitButtonTexture, NULL, &exit);
	}
	void MainMenu::act(Uint32 delay)
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		int mouseX = -1, mouseY = -1;

		if (SDL_GetMouseState(&mouseX, &mouseY) & 1) {
			mX = mouseX * game->CAMERA_WIDTH / game->GAME_WIDTH;
			mY = mouseY * game->CAMERA_HEIGHT / game->GAME_HEIGHT;
			mPressed = true;
			
		}
		else if (mPressed) {
			SDL_Point point(mX, mY);
			if (SDL_PointInRect(&point, &play)) {
				mPressed = false;
				game->setNewScreen(new Map(game, data, renderer));
				return;
			}
			if (SDL_PointInRect(&point, &exit)) {
				mPressed = false;
				SDL_Event event{};
				event.type = SDL_QUIT;
				event.quit.timestamp = SDL_GetTicks();
				SDL_PushEvent(&event);
				return;
			}
		}
	}
	SDL_Rect MainMenu::getRect()
	{
		return { 0,0,0,0 };
	}

	SDL_Point MainMenu::getFocus()
	{
		return SDL_Point();
	}

	Map::Map(Game *game, Data* data, SDL_Renderer* renderer)
		: Screen(game, data, renderer), player(0), tileNums{ nullptr }, tiles{}, winZone(0,0,0,0), curLvl(1), enemyCount(0), endgame{ false }, alfa{ 0 } {
		loadTextures(renderer, data);
		loadMapData();
	}
	Map::~Map() {
		for (size_t i = 0; i < height; i++)
		{
			delete[] tileNums[i];
		}
		delete[] tileNums;

		for (size_t i = 0; i < walls.size(); i++)
		{
			delete walls[i];
		}
		for (size_t i = 0; i < tiles.size(); i++)
		{
			SDL_DestroyTexture(tiles[i]);
		}
		for (size_t i = 0; i < enemies.size(); i++)
		{
			delete enemies[i];
		}
		SDL_DestroyTexture(deathScreen);
		SDL_DestroyTexture(winScreen);
		delete player;
	}
	void Map::render(Camera& camera, SDL_Renderer* renderer) {
		SDL_Rect pos = { 0,0,32,32 };
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				if (tileNums[i][j] < 0) {
					continue;
				}
				pos.x = j * pos.w;
				pos.y = i * pos.h;
				if (SDL_HasIntersection(&pos, &camera.rect) == SDL_TRUE) {
					pos.x -= camera.rect.x;
					pos.y -= camera.rect.y;
					SDL_RenderCopy(renderer, tiles[tileNums[i][j]], NULL, &pos);
				}
			}
		}

#ifdef DEBUG
		for (Collide* hitBox : walls)
		{
			hitBox->drawBorder(renderer, camera);
		}
#endif // DEBUG

		player->render(camera, renderer);

		for (Entity* enemy : enemies) {
			enemy->render(camera, renderer);
		}

		pos = { camera.rect.w / 2 - 330 / 2, camera.rect.h / 2 - 70 / 2, 330,70 };

		if (endgame) {
			SDL_SetTextureAlphaMod(endGameTexture, alfa);
			SDL_RenderCopy(renderer, endGameTexture, NULL, &pos);
		}
	}

	void Map::act(Uint32 delay)
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
			game->setNewScreen(new MainMenu(game, data, renderer));
			return;
		}
		player->act(delay);
		enemyCount = 0;
		for (Entity* enemy : enemies) {
			enemy->act(delay);
			if (!enemy->isDead()) {
				enemyCount++;
			}
		}
		if (endgame) {
			alfa += delay * 32 / 125;
			if (alfa > 255) {
				game->setNewScreen(new MainMenu(game, data, renderer));
			}
			return;
		}
		if (player->isDead()) {
			endgame = true;
			endGameTexture = deathScreen;
		}
		else if (player->getHitBox().checkCollision(winZone) && enemyCount == 0) {
			//WIN
			if (nextLevel()) {
				endgame = true;
				endGameTexture = winScreen;
			}
		}
	}

	SDL_Rect Map::getRect()
	{
		return SDL_Rect{ 0,0,width * 32,height * 32 };
	}
	SDL_Point Map::getFocus()
	{
		return player->getFocus();
	}
	bool Map::loadTextures(SDL_Renderer* renderer, Data* data) {
		string folder = data->tiles.place;
		for (int i = 0; i < data->tiles.count; i++) {
			string nameFile = "";
			if (i < 10) nameFile = "0";
			nameFile += std::to_string(i) + "." + data->tiles.type;

			SDL_Texture* newTexture = loadTexture(renderer, folder + nameFile);
			if (newTexture == NULL) return false;
			tiles.push_back(newTexture);
		}
		deathScreen = loadTexture(renderer, data->maps.deadPath);
		winScreen = loadTexture(renderer, data->maps.winPath);
		SDL_SetTextureBlendMode(deathScreen, SDL_BLENDMODE_BLEND);
		SDL_SetTextureBlendMode(winScreen, SDL_BLENDMODE_BLEND);

#ifdef DEBUG
		for (const auto& entry : fs::directory_iterator(folder)) {
			std::cout << entry.path() << std::endl;
		}
#endif // DEBUG
	}
	bool Map::loadMapData()
	{
		string path = data->maps.place + std::to_string(curLvl) + ".tmx";
		XMLDocument doc;
		doc.LoadFile(path.c_str());
		XMLElement* map = doc.FirstChildElement("map");
		map->QueryIntAttribute("width", &width);
		map->QueryIntAttribute("height", &height);
		if (width == 0 || height == 0) {
			isError = true;
			error = "Failed read width/height data";
		}
		else {
			tileNums = new int* [height];
			for (size_t i = 0; i < height; i++)
			{
				tileNums[i] = new int[width];
			}
			if (!enemies.empty()) {
				for (Entity* enemy : enemies) {
					delete enemy;
				}
			}
			if (!walls.empty()) {
				for (Collide* wall : walls) {
					delete wall;
				}
			}
			enemies.clear();
			walls.clear();
			loadTiles(map->FirstChildElement("layer"));
			loadObjects(map->FirstChildElement("objectgroup"), data, renderer);
		}
		return false;
	}
	bool Map::nextLevel()
	{
		curLvl++;
		if (curLvl > data->maps.count) {
			//game->setNewScreen(new MainMenu(game, data, renderer));
			return true;
		}

		if (tileNums != nullptr) {
			for (size_t i = 0; i < height; i++)
			{
				if (tileNums != nullptr)
					delete[] tileNums[i];
			}
			delete[] tileNums;
		}

		loadMapData();
		return false;
	}
	void Map::restart()
	{
		curLvl = 1;
		loadMapData();
	}
	bool Map::loadTiles(XMLElement* layer) {
		string data = layer->FirstChildElement("data")->GetText();
		if (data.empty()) return false;
		data = trim(data);
		vector<string> lines;
		split(data, lines, 10);
		if (lines.size() != height) return false;
		vector<string> buf;
		for (size_t i = 0; i < height; i++) {
			split(lines[i], buf, ',');
			if (buf.size() < width) return false;
			for (int j = 0; j < width; j++) {
				tileNums[i][j] = stoi(buf[j]) - 1;
			}
			bool a = 0;
		}
#ifdef DEBUG
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				printf("%2d, ", tileNums[i][j]);
			}
			printf("\n");
		}
#endif // DEBUG

		return true;
	}
	bool Map::loadObjects(XMLElement* objectgroup, Data* data, SDL_Renderer* r)
	{
		XMLElement* element = objectgroup->FirstChildElement("object");
		if (element == NULL) return false;
		while (element != NULL) {
			int x = 0, y = 0;
			string type = element->Attribute("type");
			if (type.empty()) return false;

			if (element->QueryIntAttribute("x", &x) == XML_NO_ATTRIBUTE) return false;
			if (element->QueryIntAttribute("y", &y) == XML_NO_ATTRIBUTE) return false;

			if (type == "rect") {
				int w = 0, h = 0;
				if (element->QueryIntAttribute("width", &w) == XML_NO_ATTRIBUTE) return false;
				if (element->QueryIntAttribute("height", &h) == XML_NO_ATTRIBUTE) return false;

				walls.push_back(new Rectangle(x, y, w, h));
			}
			else if (type == "triangle")
			{
				string name = element->Attribute("name");
				if (name == "right")
					walls.push_back(new RightTriangle(x, y - RightTriangle::SIDE));
				else
					walls.push_back(new LeftTriangle(x, y));

			}
			else if (type == "playerSpawn")
			{
				player = new Player(x, y, &data->player, walls, enemies, r);
			}
			else if (type == "enemy")  {
				enemies.push_back(new Enemy(x, y, &data->bandit[rand() % 2], walls, player, r));
				enemyCount++;
			}
			else if(type == "win") {
				int w = 0, h = 0;
				if (element->QueryIntAttribute("width", &w) == XML_NO_ATTRIBUTE) return false;
				if (element->QueryIntAttribute("height", &h) == XML_NO_ATTRIBUTE) return false;

				winZone = Rectangle(x, y, w, h);
			}
			element = element->NextSiblingElement("object");
		}
		return true;
	}
}