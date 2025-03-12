#pragma once
#include "Enums.h"
#include <SDL.h>
#include <cmath>

namespace myGame {
    /**
     * @class Camera
     * @brief A class responsible for handling camera movement in the game world.
     */
    class Camera {
    public:
        /** @brief The camera's rectangular area. */
        SDL_Rect rect;

        /** @brief Maximum movement speed of the camera along the X axis. */
        const int maxSpeedX;

        /** @brief Maximum movement speed of the camera along the Y axis. */
        const int maxSpeedY;

        /**
         * @brief Default constructor.
         * Initializes the camera with default parameters.
         */
        Camera();

        /**
         * @brief Constructor with parameters.
         * @param x Initial X coordinate.
         * @param y Initial Y coordinate.
         * @param w Camera width.
         * @param h Camera height.
         */
        Camera(int x, int y, int w, int h);

        /**
         * @brief Constructor that accepts an SDL_Rect.
         * @param rect A rectangle defining the camera's size and position.
         */
        Camera(SDL_Rect rect);

        /**
         * @brief Moves the camera based on a target point, bounds, and elapsed time.
         * @param target The point towards which the camera should move.
         * @param bounds The area within which the camera can move.
         * @param deltaTime Time elapsed since the last update.
         */
        void move(SDL_Point target, SDL_Rect bounds, Uint32 deltaTime);

        /**
         * @brief Moves the camera in a given direction while respecting bounds.
         * @param dir The direction in which the camera should move.
         * @param bounds The area within which the camera can move.
         */
        void move(Direction dir, SDL_Rect bounds);
    };
}
