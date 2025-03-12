#pragma once
#include "Enums.h"
#include "tinyxml2.h"
#include <iostream>
#include <string>

using tinyxml2::XMLElement, tinyxml2::XMLDocument, tinyxml2::XML_SUCCESS;
using std::string;

namespace myGame {
    /**
     * @struct TilesData
     * @brief Stores tile-related data parsed from XML.
     */
    struct TilesData {
        string type, place;
        int count, width, height;

        /**
         * @brief Sets tile data from an XML element.
         * @param e Pointer to the XML element.
         * @return True if data was set successfully, false otherwise.
         */
        bool set(XMLElement* e);
    };

    /**
     * @struct StateData
     * @brief Stores state-related data for animations or game states.
     */
    struct StateData {
        int count;
        string place;

        /**
         * @brief Sets state data from an XML element.
         * @param e Pointer to the XML element.
         * @return True if data was set successfully, false otherwise.
         */
        bool set(XMLElement* e);
    };

    /**
     * @struct EntityData
     * @brief Stores entity-related data including animation states.
     */
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

        /**
         * @brief Sets entity data from an XML element.
         * @param e Pointer to the XML element.
         * @return True if data was set successfully, false otherwise.
         */
        bool set(XMLElement* e);
    };

    /**
     * @struct MenuData
     * @brief Stores menu-related data such as button paths and dimensions.
     */
    struct MenuData {
        int width, height;
        string playButPath;
        string exitButPath;

        /**
         * @brief Sets menu data from an XML element.
         * @param e Pointer to the XML element.
         * @return True if data was set successfully, false otherwise.
         */
        bool set(XMLElement* e);
    };

    /**
     * @struct MapsData
     * @brief Stores map-related data including file paths for different game states.
     */
    struct MapsData {
        string type, place;
        int count;
        string winPath;
        string deadPath;

        /**
         * @brief Sets map data from an XML element.
         * @param e Pointer to the XML element.
         * @return True if data was set successfully, false otherwise.
         */
        bool set(XMLElement* e);
    };

    /**
     * @struct Data
     * @brief Stores all game-related data parsed from an XML document.
     */
    struct Data {
        TilesData tiles;
        EntityData player;
        EntityData bandit[2];
        MenuData menu;
        MapsData maps;
        string backgroundPath;

        /**
         * @brief Parses and sets all game-related data from an XML document.
         * @param d Reference to the XML document.
         * @return True if data was set successfully, false otherwise.
         */
        bool set(XMLDocument& d);
    };
}
