#pragma once
#include "Drawable.h"
#include "Enums.h"

namespace myGame {
    /**
     * @class Entity
     * @brief A base class representing all game entities.
     */
    class Entity : public Drawable {
    protected:
        float x, y; /**< Position coordinates. */
        float speedY; /**< Vertical speed of the entity. */
        int health, damage; /**< Health and damage values. */
        Direction lookDir; /**< Direction the entity is facing. */
        Direction hurtDir; /**< Direction from which the entity was hurt. */

        std::map<States, State*> states; /**< Map of entity states. */
        States currentState; /**< Current state of the entity. */

        std::vector<Collide*>& walls; /**< Reference to collidable objects. */

        Rectangle hitBox; /**< Hitbox of the entity. */
        Rectangle attackBox; /**< Attack range of the entity. */

    public:
        static const int GRAVITY = 600; /**< Gravity affecting the entity. */
        static const int MAXSPEEDY = 200; /**< Maximum falling speed. */
        static const int MAXSPEEDX = 200; /**< Maximum horizontal speed. */

        /**
         * @brief Constructor for an entity.
         * @param x X position.
         * @param y Y position.
         * @param w Width.
         * @param h Height.
         * @param data Entity data.
         * @param walls Reference to collidable objects.
         * @param renderer SDL renderer.
         */
        Entity(int x, int y, int w, int h, EntityData* data, std::vector<Collide*>& walls, SDL_Renderer* renderer);

        /**
         * @brief Destructor for Entity.
         */
        virtual ~Entity();

        float getX() const;
        float getY() const;
        float getSpeedY() const;
        void setSpeedY(float speed);
        Direction getLookDir();
        Direction getHurtDir();
        bool isDead();
        void setState(States state);
        virtual void move(Direction dir, Uint32 delay);
        void jump();
        void takeDamage(int damage, Direction dir);
        virtual void attack() = 0;
        Rectangle& getHitBox();
        bool onGround();
        void collissionUpdate();

        virtual SDL_Rect getRect() override;
        virtual SDL_Point getFocus() override;
        virtual void render(Camera& camera, SDL_Renderer* renderer) override;
    };

    /**
     * @class Player
     * @brief A class representing the player character.
     */
    class Player : public Entity {
        std::vector<Entity*>& enemies; /**< Reference to enemy entities. */
    public:
        /**
         * @brief Constructor for Player.
         * @param x X position.
         * @param y Y position.
         * @param data Entity data.
         * @param ground Reference to ground colliders.
         * @param enemies Reference to enemy entities.
         * @param renderer SDL renderer.
         */
        Player(int x, int y, EntityData* data, std::vector<Collide*>& ground, std::vector<Entity*>& enemies, SDL_Renderer* renderer);

        /**
         * @brief Destructor for Player.
         */
        virtual ~Player();

        virtual void attack() override;
        virtual void act(Uint32 delay) override;
    };

    /**
     * @class Enemy
     * @brief A class representing an enemy entity.
     */
    class Enemy : public Entity {
        Player* player; /**< Reference to the player. */
        bool targetSpotted; /**< Flag indicating if the player is detected. */
    public:
        /**
         * @brief Constructor for Enemy.
         * @param x X position.
         * @param y Y position.
         * @param data Entity data.
         * @param ground Reference to ground colliders.
         * @param player Reference to the player entity.
         * @param renderer SDL renderer.
         */
        Enemy(int x, int y, EntityData* data, std::vector<Collide*>& ground, Player* player, SDL_Renderer* renderer);

        /**
         * @brief Destructor for Enemy.
         */
        virtual ~Enemy();

        virtual void move(Direction dir, Uint32 delay) override;
        virtual void attack() override;
        virtual void act(Uint32 delay) override;
    };
}