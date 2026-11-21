#pragma once
#include <string>

namespace myGame {
	struct TilesData {
		std::string type, place;
		int count, width, height;
	};
	struct StateData {
		int count;
		std::string place;
	};
	struct EntityData {
		std::string type;
		int width, height;
		int hitW, hitH;
		int attackW, attackH;
		StateData attack;
		StateData death;
		StateData hurt;
		StateData idle;
		StateData jump;
		StateData run;
	};
	struct MenuData {
		int width, height;
		std::string playButPath;
		std::string exitButPath;

	};
	struct MapsData {
		std::string type, place;
		int count;
		std::string winPath;
		std::string deadPath;
	};
	struct Data {
		TilesData tiles;
		EntityData player;
		EntityData bandit[2];
		MenuData menu;
		MapsData maps;
		std::string backgroundPath;

		void loadFrom(const std::string& path);
	};
}
