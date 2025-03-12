#pragma once
#include "Camera.h"
#include "Collide.h"
#include "Data.h"
#include "State.h"

namespace myGame {
    /**
     * @class Drawable
     * @brief An abstract class representing objects that can be drawn and updated in the game world.
     */
    class Drawable {
    public:
        /**
         * @brief Renders the object using the provided camera and renderer.
         * @param camera The camera used for rendering.
         * @param renderer The SDL renderer.
         */
        virtual void render(Camera& camera, SDL_Renderer* renderer) = 0;

        /**
         * @brief Updates the object's state based on the elapsed time.
         * @param deltaTime The time elapsed since the last update.
         */
        virtual void act(Uint32 deltaTime) = 0;

        /**
         * @brief Retrieves the object's bounding rectangle.
         * @return The SDL_Rect representing the object's position and size.
         */
        virtual SDL_Rect getRect() = 0;

        /**
         * @brief Retrieves the focus point of the object.
         * @return The SDL_Point representing the focus position.
         */
        virtual SDL_Point getFocus() = 0;

        /**
         * @brief Virtual destructor for proper cleanup in derived classes.
         */
        virtual ~Drawable() {}
    };
}
