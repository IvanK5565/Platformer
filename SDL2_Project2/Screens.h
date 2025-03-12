#pragma once
#include "Drawable.h"
#include "Entity.h"
#include "Tools.h"
#include <filesystem>
#include <iostream>
#include <SDL_Image.h>
#include <string>
#include <vector>

namespace fs = std::filesystem;

using std::vector, std::string;

namespace myGame {
    class Game;

    /**
     * @class Screen
     * @brief Abstract base class representing a screen in the game.
     *
     * This class serves as a base for different screens in the game (e.g., main menu, game map).
     * It provides methods for rendering and handling game logic.
     */
    class Screen : public Drawable {
    protected:
        /**
         * @brief Pointer to the main game object.
         */
        Game* game;

        /**
         * @brief Pointer to the data object that holds game-related data.
         */
        Data* data;

        /**
         * @brief The renderer used to draw graphics on the screen.
         */
        SDL_Renderer* renderer;

        /**
         * @brief Constructor for initializing the screen with the game, data, and renderer.
         *
         * @param _game Pointer to the game object.
         * @param data Pointer to the data object.
         * @param r Renderer used for rendering.
         */
        Screen(Game* _game, Data* data, SDL_Renderer* r) : game(_game), data(data), renderer(r) {}
    };

    /**
     * @class MainMenu
     * @brief Represents the main menu screen of the game.
     *
     * This class is responsible for rendering and interacting with the main menu screen,
     * handling button clicks for starting the game or exiting.
     */
    class MainMenu : public Screen {
        /**
         * @brief Texture for the play button.
         */
        SDL_Texture* playButtonTexture;

        /**
         * @brief Texture for the exit button.
         */
        SDL_Texture* exitButtonTexture;

        /**
         * @brief Rectangles defining the positions and sizes of buttons.
         */
        SDL_Rect play;
        SDL_Rect exit;

        /**
         * @brief Mouse coordinates for detecting button interactions.
         */
        int mX, mY;

        /**
         * @brief Boolean to track if a button is pressed.
         */
        bool mPressed;

    public:
        /**
         * @brief Constructor for initializing the main menu.
         *
         * @param game Pointer to the game object.
         * @param data Pointer to the data object.
         * @param r Renderer used for rendering the screen.
         */
        MainMenu(Game* game, Data* data, SDL_Renderer* r);

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~MainMenu() {}

        /**
         * @brief Renders the main menu screen.
         *
         * @param camera The camera object to handle viewport rendering.
         * @param renderer The SDL renderer to draw textures.
         */
        virtual void render(Camera&, SDL_Renderer*) override;

        /**
         * @brief Handles the game logic for the main menu, such as button presses.
         *
         * @param delay The delay between frames to control the update rate.
         */
        virtual void act(Uint32) override;

        /**
         * @brief Gets the rectangle representing the main menu screen.
         *
         * @return The rectangle representing the screen area.
         */
        virtual SDL_Rect getRect() override;

        /**
         * @brief Gets the point that defines the focus of the screen.
         *
         * @return The point that represents the focus of the screen.
         */
        virtual SDL_Point getFocus() override;
    };

    /**
     * @class Map
     * @brief Represents the game map screen.
     *
     * This class is responsible for rendering the game map, handling tile data,
     * managing enemies, and processing game events such as winning or losing.
     */
    class Map : public Screen {
        /**
         * @brief Current level of the map.
         */
        int curLvl;

        /**
         * @brief Width and height of the map.
         */
        int width, height;

        /**
         * @brief 2D array of tile numbers representing the map.
         */
        int** tileNums;

        /**
         * @brief The number of enemies in the level.
         */
        int enemyCount;

        /**
         * @brief List of walls (collidable objects) in the map.
         */
        vector<Collide*> walls;

        /**
         * @brief List of textures for map tiles.
         */
        vector<SDL_Texture*> tiles;

        /**
         * @brief List of enemy entities in the map.
         */
        vector<Entity*> enemies;

        /**
         * @brief Pointer to the player entity.
         */
        Player* player;

        /**
         * @brief The rectangle representing the win zone on the map.
         */
        Rectangle winZone;

        /**
         * @brief Textures for displaying game-over and win screens.
         */
        SDL_Texture* deathScreen;
        SDL_Texture* winScreen;
        SDL_Texture* endGameTexture;

        /**
         * @brief Boolean indicating if the game has ended.
         */
        bool endgame;

        /**
         * @brief Transparency value for certain effects.
         */
        Uint16 alfa;

    public:
        /**
         * @brief Error message for loading the map.
         */
        string error;

        /**
         * @brief Boolean indicating if there is an error loading the map.
         */
        bool isError;

        /**
         * @brief Constructor for initializing the map screen.
         *
         * @param game Pointer to the game object.
         * @param data Pointer to the data object.
         * @param r Renderer used for rendering the screen.
         */
        Map(Game*, Data*, SDL_Renderer*);

        /**
         * @brief Destructor for cleaning up map resources.
         */
        virtual ~Map();

        /**
         * @brief Renders the map on the screen.
         *
         * @param camera The camera object to handle viewport rendering.
         * @param renderer The SDL renderer to draw textures.
         */
        virtual void render(Camera&, SDL_Renderer*) override;

        /**
         * @brief Handles the game logic for the map, including enemy actions and player movement.
         *
         * @param delay The delay between frames to control the update rate.
         */
        virtual void act(Uint32) override;

        /**
         * @brief Gets the rectangle representing the map screen.
         *
         * @return The rectangle representing the screen area.
         */
        virtual SDL_Rect getRect() override;

        /**
         * @brief Gets the point that defines the focus of the map screen.
         *
         * @return The point that represents the focus of the screen.
         */
        virtual SDL_Point getFocus() override;

    private:
        /**
         * @brief Loads the tiles for the map from an XML element.
         *
         * @param elem The XML element containing tile data.
         * @return True if tiles were loaded successfully, false otherwise.
         */
        bool loadTiles(XMLElement*);

        /**
         * @brief Loads game objects (like enemies, items) from an XML element.
         *
         * @param elem The XML element containing object data.
         * @param data The data object used for managing game objects.
         * @param r The SDL renderer used for rendering objects.
         * @return True if objects were loaded successfully, false otherwise.
         */
        bool loadObjects(XMLElement*, Data*, SDL_Renderer*);

        /**
         * @brief Loads textures for the map and its objects.
         *
         * @param r The SDL renderer used for rendering.
         * @param data The data object used for managing textures.
         * @return True if textures were loaded successfully, false otherwise.
         */
        bool loadTextures(SDL_Renderer*, Data*);

        /**
         * @brief Loads map data (such as tile positions, enemies).
         *
         * @return True if map data was loaded successfully, false otherwise.
         */
        bool loadMapData();

        /**
         * @brief Advances to the next level in the map.
         *
         * @return True if the next level was loaded successfully, false otherwise.
         */
        bool nextLevel();

        /**
         * @brief Restarts the current level.
         */
        void restart();
    };
}
