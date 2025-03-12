#pragma once
#include "Enums.h"
#include "Camera.h"
#include <SDL.h>

namespace myGame {

    class Entity;
    class Rectangle;

    /**
     * @struct CollisionInfo
     * @brief Stores information about a collision between objects.
     */
    struct CollisionInfo {
        bool collision; ///< Indicates whether a collision occurred.
        int x1; ///< Distance between the right side of A and the left side of B.
        int x2; ///< Distance between the left side of A and the right side of B.
        int y1; ///< Distance between the bottom side of A and the top side of B.
        int y2; ///< Distance between the top side of A and the bottom side of B.

        /**
         * @brief Converts CollisionInfo to a boolean value.
         * @return True if a collision occurred, false otherwise.
         */
        explicit operator bool() const {
            return collision;
        }
    };

    /**
     * @class Collide
     * @brief Base class for collision detection.
     */
    class Collide {
    public:
        int x, y; ///< Position of the object.

        /**
         * @brief Constructor.
         * @param x Initial X position.
         * @param y Initial Y position.
         */
        Collide(int x, int y) : x(x), y(y) {}

        /**
         * @brief Sets the object's position.
         * @param newX New X position.
         * @param newY New Y position.
         */
        void setPos(int newX, int newY);

        virtual ~Collide() {}

        /**
         * @brief Checks for collision with a rectangle.
         * @param rect The rectangle to check collision with.
         * @return Collision information.
         */
        virtual CollisionInfo checkCollision(const Rectangle& rect) = 0;

        /**
         * @brief Checks for collision at a specific point.
         * @param _x X coordinate.
         * @param _y Y coordinate.
         * @return Collision information.
         */
        virtual CollisionInfo checkCollision(const int _x, const int _y) = 0;

        /**
         * @brief Draws the collision border.
         * @param renderer SDL renderer.
         * @param camera The camera view.
         */
        virtual void drawBorder(SDL_Renderer* renderer, Camera& camera) = 0;

        /**
         * @brief Gets the type of collision object.
         * @return Collision type.
         */
        virtual CollideType getType() = 0;
    };

    /**
     * @class RightTriangle
     * @brief Represents a right-angled triangle collision object.
     */
    class RightTriangle : public Collide {
    public:
        static const int SIDE = 32; ///< Side length of the triangle.

        /**
         * @brief Constructor.
         * @param _x X position.
         * @param _y Y position.
         */
        RightTriangle(int _x, int _y) : Collide(_x, _y) {}
        virtual ~RightTriangle() {}

        virtual CollisionInfo checkCollision(const Rectangle&) override;
        virtual CollisionInfo checkCollision(const int _x, const int _y) override;
        virtual void drawBorder(SDL_Renderer*, Camera&) override;
        virtual CollideType getType() override;
    };

    /**
     * @class LeftTriangle
     * @brief Represents a left-angled triangle collision object.
     */
    class LeftTriangle : public Collide {
    public:
        static const int SIDE = 32; ///< Side length of the triangle.

        /**
         * @brief Constructor.
         * @param _x X position.
         * @param _y Y position.
         */
        LeftTriangle(int _x, int _y) : Collide(_x, _y) {}
        virtual ~LeftTriangle() {}

        virtual CollisionInfo checkCollision(const Rectangle&) override;
        virtual CollisionInfo checkCollision(const int _x, const int _y) override;
        virtual void drawBorder(SDL_Renderer*, Camera&) override;
        virtual CollideType getType() override;
    };

    /**
     * @class Rectangle
     * @brief Represents a rectangular collision object.
     */
    class Rectangle : public Collide {
    public:
        int width, height; ///< Width and height of the rectangle.

        /**
         * @brief Constructor.
         * @param x X position.
         * @param y Y position.
         * @param w Width.
         * @param h Height.
         */
        Rectangle(int x, int y, int w, int h) : Collide(x, y), width(w), height(h) {}

        /**
         * @brief Constructor using an SDL_Rect.
         * @param rect SDL rectangle.
         */
        Rectangle(SDL_Rect rect) : Collide(rect.x, rect.y), width(rect.w), height(rect.h) {}

        virtual ~Rectangle() {}

        virtual CollisionInfo checkCollision(const Rectangle&) override;
        virtual CollisionInfo checkCollision(const int _x, const int _y) override;
        virtual void drawBorder(SDL_Renderer*, Camera&) override;
        virtual CollideType getType() override;

        /**
         * @brief Gets the SDL_Rect representation of the rectangle.
         * @return SDL_Rect with position and size.
         */
        SDL_Rect getRect() {
            return { x, y, width, height };
        }

        friend class RightTriangle;
        friend class LeftTriangle;
    };
}
