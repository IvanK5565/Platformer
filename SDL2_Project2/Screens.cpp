#include "Screens.h"
namespace myGame {
	void MainMenu::handleEvent(SDL_Event& e)
	{
	}
	void MainMenu::render(Camera& camera, SDL_Renderer* renderer) {

	}
	void MainMenu::act(Uint32 delay)
	{

	}
	SDL_Rect MainMenu::getRect()
	{
		return { 0,0,0,0 };
	}

	SDL_Point MainMenu::getFocus()
	{
		return SDL_Point();
	}

	Map::Map(Data* data, SDL_Renderer* renderer) : player(0), tileNums{ nullptr }, tiles{} {
		string path = data->maps.place + "1.tmx";
		TiXmlDocument doc(path.c_str());
		doc.LoadFile();
		TiXmlElement* map = doc.FirstChildElement("map");
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
			loadTiles(map->FirstChildElement("layer"));
			loadObjects(map->FirstChildElement("objectgroup"), data, renderer);
			loadTextures(renderer, data);
		}
	}
	void Map::handleEvent(SDL_Event& e)
	{
		player->handleEvent(e);
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
		pos.x = 0;
		pos.y = 0;
		SDL_RenderCopy(renderer, tiles[0], NULL, &pos);

#ifdef DEBUG
		for (Collide* hitBox : collides)
		{
			hitBox->drawBorder(renderer, camera);
		}
#endif // DEBUG

		player->render(camera, renderer);
	}

	void Map::act(Uint32 delay)
	{
		player->act(delay);
		for (Entity* enemy : enemies) {
			enemy->act(delay);
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

#ifdef DEBUG
		for (const auto& entry : fs::directory_iterator(folder)) {
			std::cout << entry.path() << std::endl;
		}
#endif // DEBUG
	}
	bool Map::loadTiles(TiXmlElement* layer) {
		string data = layer->FirstChildElement("data")->GetText();
		if (data.empty()) return false;
		vector<string> lines;
		split(data, lines, 32);
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
	bool Map::loadObjects(TiXmlElement* objectgroup, Data* data, SDL_Renderer* r)
	{
		TiXmlElement* element = objectgroup->FirstChildElement("object");
		if (element == NULL) return false;
		while (element != NULL) {
			int x = 0, y = 0;
			string type = element->Attribute("type");
			if (type.empty()) return false;

			if (element->QueryIntAttribute("x", &x) == TIXML_NO_ATTRIBUTE) return false;
			if (element->QueryIntAttribute("y", &y) == TIXML_NO_ATTRIBUTE) return false;

			if (type == "rect") {
				int w = 0, h = 0;
				if (element->QueryIntAttribute("width", &w) == TIXML_NO_ATTRIBUTE) return false;
				if (element->QueryIntAttribute("height", &h) == TIXML_NO_ATTRIBUTE) return false;

				collides.push_back(new Square(x, y, w, h));
			}
			else if (type == "triangle")
			{
				string name = element->Attribute("name");
				if (name == "right")
					collides.push_back(new RightTriangle(x, y - Collide::SIDE));
				else
					collides.push_back(new LeftTriangle(x, y));

			}
			else if (type == "playerSpawn")
			{
				player = new Player(x, y, &data->player, collides, enemies, r);
			}
			else {

			}
			element = element->NextSiblingElement("object");
		}
		return true;
	}
}