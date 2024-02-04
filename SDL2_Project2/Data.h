#pragma once
#include "Enums.h"
#include <string>
#include <iostream>
#include <tinyxml.h>

using std::string;

namespace myGame {
	struct TilesData {
		string type, place;
		int count, width, height;
		bool set(TiXmlElement* e) {
			type = e->Attribute("type");
			if (type.empty()) return false;
			place = e->Attribute("place");
			if (place.empty()) return false;
			if (e->QueryIntAttribute("count", &count) != TIXML_SUCCESS) return false;
			if (e->QueryIntAttribute("width", &width) != TIXML_SUCCESS) return false;
			if (e->QueryIntAttribute("height", &height) != TIXML_SUCCESS) return false;
			return true;
		}
	};
	struct Test {

	};
	struct StateData {
		int count;
		string place;
		bool set(TiXmlElement* e) {
			place = e->Attribute("place");
			if (place.empty()) return false;
			if (e->QueryIntAttribute("count", &count) != TIXML_SUCCESS) return false;
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
		bool set(TiXmlElement* e) {
			type = e->Attribute("type");
			if (type.empty()) return false;
			if (e->QueryIntAttribute("width", &width) != TIXML_SUCCESS) return false;
			if (e->QueryIntAttribute("height", &height) != TIXML_SUCCESS) return false;
			if (e->QueryIntAttribute("hitW", &hitW) != TIXML_SUCCESS) return false;
			if (e->QueryIntAttribute("hitH", &hitH) != TIXML_SUCCESS) return false;
			if (e->QueryIntAttribute("attackW", &attackW) != TIXML_SUCCESS) return false;
			if (e->QueryIntAttribute("attackH", &attackH) != TIXML_SUCCESS) return false;

			TiXmlElement* e2;

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
	struct MapsData {
		string type, place;
		int count;
		bool set(TiXmlElement* e) {
			type = e->Attribute("type");
			if (type.empty()) return false;
			place = e->Attribute("place");
			if (place.empty()) return false;
			if (e->QueryIntAttribute("count", &count) != TIXML_SUCCESS) return false;
			return true;
		}
	};
	struct Data {
		TilesData tiles;
		EntityData player;
		EntityData enemy;
		MapsData maps;
		string backgroundPath;
		bool set(TiXmlDocument& d) {
			TiXmlElement* data = d.FirstChildElement("data");
			TiXmlElement* e;

			e = data->FirstChildElement("tiles");
			if (e == nullptr) return false;
			if (!tiles.set(e)) return false;

			e = data->FirstChildElement("player");
			if (e == nullptr) return false;
			if (!player.set(e)) return false;

			e = data->FirstChildElement("maps");
			if (e == nullptr) return false;
			if (!maps.set(e)) return false;

			e = data->FirstChildElement("background");
			if (e == nullptr) return false;
			backgroundPath = e->Attribute("path");
			if (backgroundPath.empty()) return false;
		}
	};
}