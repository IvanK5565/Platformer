#include "Data.h"
#include "tinyxml2.h"
#include <stdexcept>
#include <utility>

namespace {
	using tinyxml2::XMLElement, tinyxml2::XMLDocument, tinyxml2::XML_SUCCESS;

	bool readRequiredAttribute(const XMLElement& e, const char* name, std::string& value) {
		const char* attribute = e.Attribute(name);
		if (attribute == nullptr || *attribute == '\0') return false;
		value = attribute;
		return true;
	}

	bool setTiles(myGame::TilesData& tiles, XMLElement* e) {
		if (!readRequiredAttribute(*e, "type", tiles.type)) return false;
		if (!readRequiredAttribute(*e, "place", tiles.place)) return false;
		if (e->QueryIntAttribute("count", &(tiles.count)) != XML_SUCCESS) return false;
		if (e->QueryIntAttribute("width", &(tiles.width)) != XML_SUCCESS) return false;
		if (e->QueryIntAttribute("height", &(tiles.height)) != XML_SUCCESS) return false;
		return true;
	}

	bool setState(myGame::StateData& state, XMLElement* e) {
		if (!readRequiredAttribute(*e, "place", state.place)) return false;
		if (e->QueryIntAttribute("count", &(state.count)) != XML_SUCCESS) return false;
		return true;
	}

	bool setEntity(myGame::EntityData& entity, XMLElement* e) {
		if (!readRequiredAttribute(*e, "type", entity.type)) return false;
		if (e->QueryIntAttribute("width", &(entity.width)) != XML_SUCCESS) return false;
		if (e->QueryIntAttribute("height", &(entity.height)) != XML_SUCCESS) return false;
		if (e->QueryIntAttribute("hitW", &(entity.hitW)) != XML_SUCCESS) return false;
		if (e->QueryIntAttribute("hitH", &(entity.hitH)) != XML_SUCCESS) return false;
		if (e->QueryIntAttribute("attackW", &(entity.attackW)) != XML_SUCCESS) return false;
		if (e->QueryIntAttribute("attackH", &(entity.attackH)) != XML_SUCCESS) return false;

		XMLElement* e2;

		e2 = e->FirstChildElement("attack");
		if (e2 == nullptr) return false;
		if (!setState(entity.attack, e2)) return false;

		e2 = e->FirstChildElement("death");
		if (e2 == nullptr) return false;
		if (!setState(entity.death, e2)) return false;

		e2 = e->FirstChildElement("hurt");
		if (e2 == nullptr) return false;
		if (!setState(entity.hurt, e2)) return false;

		e2 = e->FirstChildElement("idle");
		if (e2 == nullptr) return false;
		if (!setState(entity.idle, e2)) return false;

		e2 = e->FirstChildElement("jump");
		if (e2 == nullptr) return false;
		if (!setState(entity.jump, e2)) return false;

		e2 = e->FirstChildElement("run");
		if (e2 == nullptr) return false;
		if (!setState(entity.run, e2)) return false;

		return true;

	}

	bool setMenu(myGame::MenuData& menu, XMLElement* e) {
		if (e->QueryIntAttribute("width", &(menu.width)) != XML_SUCCESS) return false;
		if (e->QueryIntAttribute("height", &(menu.height)) != XML_SUCCESS) return false;


		XMLElement* e2;

		e2 = e->FirstChildElement("play");
		if (e2 == nullptr) return false;
		if (!readRequiredAttribute(*e2, "path", menu.playButPath)) return false;

		e2 = e->FirstChildElement("exit");
		if (e2 == nullptr) return false;
		if (!readRequiredAttribute(*e2, "path", menu.exitButPath)) return false;

		return true;
	}

	bool setMaps(myGame::MapsData& maps, XMLElement* e) {
		if (!readRequiredAttribute(*e, "type", maps.type)) return false;
		if (!readRequiredAttribute(*e, "place", maps.place)) return false;
		if (e->QueryIntAttribute("count", &(maps.count)) != XML_SUCCESS) return false;
		if (!readRequiredAttribute(*e, "winPath", maps.winPath)) return false;
		if (!readRequiredAttribute(*e, "deadPath", maps.deadPath)) return false;
		return true;
	}

	bool setData(myGame::Data& result, XMLDocument& d) {
		XMLElement* data = d.FirstChildElement("data");
		if (data == nullptr) return false;

		XMLElement* e;
		e = data->FirstChildElement("tiles");
		if (e == nullptr || !setTiles(result.tiles, e)) return false;
		e = data->FirstChildElement("player");
		if (e == nullptr || !setEntity(result.player, e)) return false;
		e = data->FirstChildElement("bandit1");
		if (e == nullptr || !setEntity(result.bandit[0], e)) return false;
		e = data->FirstChildElement("bandit2");
		if (e == nullptr || !setEntity(result.bandit[1], e)) return false;
		e = data->FirstChildElement("menu");
		if (e == nullptr || !setMenu(result.menu, e)) return false;
		e = data->FirstChildElement("maps");
		if (e == nullptr || !setMaps(result.maps, e)) return false;
		e = data->FirstChildElement("background");
		return e != nullptr && readRequiredAttribute(*e, "path", result.backgroundPath);
	}
}

namespace myGame {
	void Data::init(const std::string& path) {
		XMLDocument doc;
		if (doc.LoadFile(path.c_str()) != XML_SUCCESS) {
			throw std::runtime_error("Could not load " + path + ": " + doc.ErrorStr());
		}

		Data parsed{};
		if (!setData(parsed, doc)) {
			throw std::runtime_error("Invalid game configuration: " + path);
		}
		*this = std::move(parsed);
	}
}
