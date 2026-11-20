#pragma once
#include "tinyxml2.h"
#include <string>

using tinyxml2::XMLElement, tinyxml2::XMLDocument, tinyxml2::XML_SUCCESS;
using std::string;

namespace myGame {
	inline bool readRequiredAttribute(const XMLElement& element, const char* name, string& value) {
		const char* attribute = element.Attribute(name);
		if (attribute == nullptr || *attribute == '\0') return false;
		value = attribute;
		return true;
	}

	struct TilesData {
		string type, place;
		int count, width, height;
		bool set(XMLElement* e) {
			if (!readRequiredAttribute(*e, "type", type)) return false;
			if (!readRequiredAttribute(*e, "place", place)) return false;
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
			if (!readRequiredAttribute(*e, "place", place)) return false;
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
			if (!readRequiredAttribute(*e, "type", type)) return false;
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
			if (!readRequiredAttribute(*e2, "path", playButPath)) return false;
			
			e2 = e->FirstChildElement("exit");
			if (e2 == nullptr) return false;
			if (!readRequiredAttribute(*e2, "path", exitButPath)) return false;

			return true;
		}

	};
	struct MapsData {
		string type, place;
		int count;
		string winPath;
		string deadPath;
		bool set(XMLElement* e) {
			if (!readRequiredAttribute(*e, "type", type)) return false;
			if (!readRequiredAttribute(*e, "place", place)) return false;
			if (e->QueryIntAttribute("count", &count) != XML_SUCCESS) return false;
			if (!readRequiredAttribute(*e, "winPath", winPath)) return false;
			if (!readRequiredAttribute(*e, "deadPath", deadPath)) return false;
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
			if (data == nullptr) return false;

			XMLElement* e;
			e = data->FirstChildElement("tiles");
			if (e == nullptr || !tiles.set(e)) return false;
			e = data->FirstChildElement("player");
			if (e == nullptr || !player.set(e)) return false;
			e = data->FirstChildElement("bandit1");
			if (e == nullptr || !bandit[0].set(e)) return false;
			e = data->FirstChildElement("bandit2");
			if (e == nullptr || !bandit[1].set(e)) return false;
			e = data->FirstChildElement("menu");
			if (e == nullptr || !menu.set(e)) return false;
			e = data->FirstChildElement("maps");
			if (e == nullptr || !maps.set(e)) return false;
			e = data->FirstChildElement("background");
			return e != nullptr && readRequiredAttribute(*e, "path", backgroundPath);
		}
	};
}
