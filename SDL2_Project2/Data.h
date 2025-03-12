#pragma once
#include "Enums.h"
#include "tinyxml2.h"
#include <iostream>
#include <string>

using tinyxml2::XMLElement, tinyxml2::XMLDocument, tinyxml2::XML_SUCCESS;
using std::string;

namespace myGame {
	struct TilesData {
		string type, place;
		int count, width, height;
		bool set(XMLElement* e) {
			type = e->Attribute("type");
			if (type.empty()) return false;
			place = e->Attribute("place");
			if (place.empty()) return false;
			if (e->QueryIntAttribute("count", &count) != XML_SUCCESS) return false;
			if (e->QueryIntAttribute("width", &width) != XML_SUCCESS) return false;
			if (e->QueryIntAttribute("height", &height) != XML_SUCCESS) return false;
			return true;
		}
	};
	struct StateData {
		int count;
		string place;
		bool set(XMLElement* e) {
			place = e->Attribute("place");
			if (place.empty()) return false;
			if (e->QueryIntAttribute("count", &count) != XML_SUCCESS) return false;
			return true;
		}
	};
	struct EntityData {
		string type;
		int width, height;
		int hitW, hitH;
		int attackW, attackH;
		StateData attack;
		StateData death;
		StateData hurt;
		StateData idle;
		StateData jump;
		StateData run;
		bool set(XMLElement* e) {
			type = e->Attribute("type");
			if (type.empty()) return false;
			if (e->QueryIntAttribute("width", &width) != XML_SUCCESS) return false;
			if (e->QueryIntAttribute("height", &height) != XML_SUCCESS) return false;
			if (e->QueryIntAttribute("hitW", &hitW) != XML_SUCCESS) return false;
			if (e->QueryIntAttribute("hitH", &hitH) != XML_SUCCESS) return false;
			if (e->QueryIntAttribute("attackW", &attackW) != XML_SUCCESS) return false;
			if (e->QueryIntAttribute("attackH", &attackH) != XML_SUCCESS) return false;

			XMLElement* e2;

			e2 = e->FirstChildElement("attack");
			if (e2 == nullptr) return false;
			if (!attack.set(e2)) return false;

			e2 = e->FirstChildElement("death");
			if (e2 == nullptr) return false;
			if (!death.set(e2)) return false;

			e2 = e->FirstChildElement("hurt");
			if (e2 == nullptr) return false;
			if (!hurt.set(e2)) return false;

			e2 = e->FirstChildElement("idle");
			if (e2 == nullptr) return false;
			if (!idle.set(e2)) return false;

			e2 = e->FirstChildElement("jump");
			if (e2 == nullptr) return false;
			if (!jump.set(e2)) return false;

			e2 = e->FirstChildElement("run");
			if (e2 == nullptr) return false;
			if (!run.set(e2)) return false;

			return true;

		}
	};
	struct MenuData {
		int width, height;
		string playButPath;
		string exitButPath;
		bool set(XMLElement* e) {
			if (e->QueryIntAttribute("width", &width) != XML_SUCCESS) return false;
			if (e->QueryIntAttribute("height", &height) != XML_SUCCESS) return false;


			XMLElement* e2;

			e2 = e->FirstChildElement("play");
			if (e2 == nullptr) return false;
			playButPath = e2->Attribute("path");
			if (playButPath.empty()) return false;
			
			e2 = e->FirstChildElement("exit");
			if (e2 == nullptr) return false;
			exitButPath = e2->Attribute("path");
			if (exitButPath.empty()) return false;

			return true;
		}

	};
	struct MapsData {
		string type, place;
		int count;
		string winPath;
		string deadPath;
		bool set(XMLElement* e) {
			type = e->Attribute("type");
			if (type.empty()) return false;
			place = e->Attribute("place");
			if (place.empty()) return false;
			if (e->QueryIntAttribute("count", &count) != XML_SUCCESS) return false;
			winPath = e->Attribute("winPath");
			if (winPath.empty()) return false;
			deadPath = e->Attribute("deadPath");
			if (deadPath.empty()) return false;
			return true;
		}
	};
	struct Data {
		TilesData tiles;
		EntityData player;
		EntityData bandit[2];
		MenuData menu;
		MapsData maps;
		string backgroundPath;
		bool set(XMLDocument& d) {
			XMLElement* data = d.FirstChildElement("data");
			XMLElement* e;
			e = data->FirstChildElement("tiles");
			tiles.set(e);
			e = data->FirstChildElement("player");
			player.set(e);
			e = data->FirstChildElement("bandit1");
			bandit[0].set(e);
			e = data->FirstChildElement("bandit2");
			bandit[1].set(e);
			e = data->FirstChildElement("menu");
			menu.set(e);
			e = data->FirstChildElement("maps");
			maps.set(e);
			e = data->FirstChildElement("background");
			backgroundPath = e->Attribute("path");
			return true;
		}
	};
}