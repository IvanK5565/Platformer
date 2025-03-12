#pragma once
#include "Camera.h"
#include "Data.h"
#include "Tools.h"
#include <SDL_ttf.h>
#include <windows.h>
#include <string>

using std::string;

namespace myGame {

    class Screen;

    /**
     * @class Game
     * @brief The main class that represents the game, managing window, rendering, and game states.
     *
     * This class is responsible for initializing the game, managing the screen, handling events,
     * rendering content, and controlling the game flow.
     */
    class Game {
    private:
        /**
         * @brief Path to the font file used in the game.
         */
        const string fontPath = "./assets/fonts/monogram.ttf";

        /**
         * @brief The SDL_Window used for the game window.
         */
        SDL_Window* window;

        /**
         * @brief The SDL_Renderer used for rendering graphics.
         */
        SDL_Renderer* renderer;

        /**
         * @brief Font used for rendering text.
         */
        TTF_Font* font;

        /**
         * @brief Texture for the loading screen.
         */
        SDL_Texture* loadingTexture;

        /**
         * @brief Background texture for the game screen.
         */
        SDL_Texture* background;

        /**
         * @brief Camera that handles the viewport and scrolling.
         */
        Camera camera;

        /**
         * @brief Pointer to the current screen object.
         */
        Screen* screen;

        /**
         * @brief Data object that manages game data.
         */
        Data data;

        /**
         * @brief Private constructor for initializing the game with default settings.
         */
        Game();

    public:
        /**
         * @brief The width of the screen.
         */
        const int SCREEN_WIDTH, SCREEN_HEIGHT;

        /**
         * @brief The game width (resolution for the game).
         */
        const int GAME_WIDTH, GAME_HEIGHT;

        /**
         * @brief The width of the camera's viewport.
         */
        const int CAMERA_WIDTH, CAMERA_HEIGHT;

        /**
         * @brief Boolean indicating if the game should quit.
         */
        bool _quit;

        /**
         * @brief Constructor for initializing the game with specific screen width and height.
         *
         * @param width The width of the screen.
         * @param height The height of the screen.
         */
        Game(int width, int height);

        /**
         * @brief Loads the main menu screen.
         *
         * This method initializes and loads the assets for the main menu.
         */
        void loadMainMenu();

        /**
         * @brief Sets a new screen for the game.
         *
         * This method sets the given screen as the active screen in the game.
         *
         * @param scr Pointer to the new screen object.
         */
        void setNewScreen(Screen* scr);

        /**
         * @brief Main game loop that handles game updates.
         *
         * This method handles the game logic, updates, and events for each frame.
         *
         * @param delay Time delay between frames to control the game's frame rate.
         */
        void act(Uint32 delay);

        /**
         * @brief Handles SDL events (e.g., keyboard, mouse, etc.).
         *
         * This method processes user input events such as key presses and mouse movements.
         *
         * @param e SDL event to be processed.
         */
        void handleEvent(SDL_Event& e);

        /**
         * @brief Renders the game content on the screen.
         *
         * This method renders the game's graphics, including textures and UI elements.
         */
        void render();

        /**
         * @brief Exits the game.
         *
         * This method handles the quit process, including cleanup and shutdown.
         *
         * @return An integer representing the exit code.
         */
        int quit();
    };
}
