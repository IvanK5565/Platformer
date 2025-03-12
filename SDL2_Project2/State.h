#pragma once
#include "Data.h"
#include "Enums.h"
#include "Tools.h"
#include <map>
#include <SDL.h>
#include <vector>
using std::map, std::vector;

namespace myGame {

    class Entity;

    /**
     * @class State
     * @brief Represents a generic state for an entity (player or enemy).
     *
     * This class is the base class for all states an entity can be in, such as idle, running, jumping, etc.
     * It handles frame animations, rendering, and state transitions.
     */
    class State {
    protected:
        /**
         * @brief The width and height of the state (e.g., sprite size).
         */
        int w, h;

        /**
         * @brief The entity this state belongs to.
         */
        Entity* entity;

        /**
         * @brief A vector of textures for each frame in the animation.
         */
        vector<SDL_Texture*> frames;

        /**
         * @brief The index of the current frame in the animation.
         */
        int currentFrame;

        /**
         * @brief Timer for controlling the frame rate of the animation.
         */
        Uint32 timer;

        /**
         * @brief Direction for flipping the sprite (used for mirroring the sprite).
         */
        SDL_RendererFlip flip;

    public:
        /**
         * @brief Constructor for initializing the state with the entity, dimensions, data, type, and renderer.
         *
         * @param e Pointer to the entity the state belongs to.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state (e.g., "Idle", "Run").
         * @param r The renderer for drawing textures.
         */
        State(Entity* e, int _w, int _h, StateData* data, string type, SDL_Renderer* r);

        /**
         * @brief Executes the state logic (e.g., updating the animation frame).
         *
         * @param delay The delay between frames, used to control animation speed.
         */
        virtual void act(Uint32 delay);

        /**
         * @brief Resets the state to its initial conditions (e.g., resets animation frames).
         */
        virtual void reset();

        /**
         * @brief Renders the state (e.g., draws the current frame of the sprite).
         *
         * @param camera The camera object to handle viewport rendering.
         * @param renderer The SDL renderer to draw textures.
         */
        void render(Camera& camera, SDL_Renderer* renderer);

        /**
         * @brief Checks if the current frame is the last frame of the animation.
         *
         * @return True if the current frame is the last, otherwise false.
         */
        bool isLastFrame();

        /**
         * @brief Destructor to clean up any allocated resources for the state.
         */
        virtual ~State();
    };

    // Player States

    /**
     * @class IdleState
     * @brief Represents the idle state of the player.
     *
     * In this state, the player is standing still and not performing any action.
     */
    class IdleState : public State {
    public:
        /**
         * @brief Constructor for initializing the idle state of the player.
         *
         * @param entity Pointer to the player entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Idle").
         * @param renderer The renderer for drawing textures.
         */
        IdleState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer) {
        }

        /**
         * @brief Executes the idle state logic (e.g., updating the idle animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~IdleState() {}
    };

    /**
     * @class RunState
     * @brief Represents the running state of the player.
     *
     * In this state, the player is moving forward.
     */
    class RunState : public State {
    public:
        /**
         * @brief Constructor for initializing the running state of the player.
         *
         * @param entity Pointer to the player entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Run").
         * @param renderer The renderer for drawing textures.
         */
        RunState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer) {
        }

        /**
         * @brief Executes the running state logic (e.g., updating the running animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~RunState() {}
    };

    /**
     * @class JumpState
     * @brief Represents the jumping state of the player.
     *
     * In this state, the player is in the air, having jumped.
     */
    class JumpState : public State {
    public:
        /**
         * @brief Constructor for initializing the jumping state of the player.
         *
         * @param entity Pointer to the player entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Jump").
         * @param renderer The renderer for drawing textures.
         */
        JumpState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer) {
        }

        /**
         * @brief Executes the jumping state logic (e.g., updating the jump animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~JumpState() {}
    };

    /**
     * @class AttackState
     * @brief Represents the attacking state of the player.
     *
     * In this state, the player is performing an attack (e.g., swinging a weapon).
     */
    class AttackState : public State {
        bool ended;  ///< Boolean to track if the attack has ended.
        bool cooldown;  ///< Boolean to track if the attack is on cooldown.
    public:
        /**
         * @brief Constructor for initializing the attacking state of the player.
         *
         * @param entity Pointer to the player entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Attack").
         * @param renderer The renderer for drawing textures.
         */
        AttackState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer), ended{ false }, cooldown{ false } {
        }

        /**
         * @brief Resets the attack state (e.g., sets the cooldown back).
         */
        virtual void reset() override;

        /**
         * @brief Executes the attack state logic (e.g., updating the attack animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~AttackState() {}
    };

    /**
     * @class HurtState
     * @brief Represents the hurt state of the player.
     *
     * In this state, the player is injured and may be in a brief stunned animation.
     */
    class HurtState : public State {
        unsigned int hurtTimer;  ///< Timer for controlling the duration of the hurt state.
    public:
        /**
         * @brief Constructor for initializing the hurt state of the player.
         *
         * @param entity Pointer to the player entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Hurt").
         * @param renderer The renderer for drawing textures.
         */
        HurtState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer), hurtTimer{ 0 } {
        }

        /**
         * @brief Resets the hurt state (e.g., reset the hurt timer).
         */
        virtual void reset() override;

        /**
         * @brief Executes the hurt state logic (e.g., updating the hurt animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~HurtState() {}
    };

    /**
     * @class DeathState
     * @brief Represents the death state of the player.
     *
     * In this state, the player is dead and may play a death animation or trigger game over.
     */
    class DeathState : public State {
    public:
        /**
         * @brief Constructor for initializing the death state of the player.
         *
         * @param entity Pointer to the player entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Death").
         * @param renderer The renderer for drawing textures.
         */
        DeathState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer) {
        }

        /**
         * @brief Executes the death state logic (e.g., updating the death animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~DeathState() {}
    };

    // Enemy States

/**
 * @class EnemyIdleState
 * @brief Represents the idle state of an enemy.
 *
 * In this state, the enemy is standing still and not performing any action.
 */
    class EnemyIdleState : public State {
    public:
        /**
         * @brief Constructor for initializing the enemy's idle state.
         *
         * @param entity Pointer to the enemy entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Idle").
         * @param renderer The renderer for drawing textures.
         */
        EnemyIdleState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer) {
        }

        /**
         * @brief Executes the idle state logic (e.g., updating the idle animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~EnemyIdleState() {}
    };

    /**
     * @class EnemyRunState
     * @brief Represents the running state of an enemy.
     *
     * In this state, the enemy is moving forward.
     */
    class EnemyRunState : public State {
    public:
        /**
         * @brief Constructor for initializing the enemy's running state.
         *
         * @param entity Pointer to the enemy entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Run").
         * @param renderer The renderer for drawing textures.
         */
        EnemyRunState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer) {
        }

        /**
         * @brief Executes the running state logic (e.g., updating the run animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~EnemyRunState() {}
    };

    /**
     * @class EnemyJumpState
     * @brief Represents the jumping state of an enemy.
     *
     * In this state, the enemy is in the air, having jumped.
     */
    class EnemyJumpState : public State {
    public:
        /**
         * @brief Constructor for initializing the enemy's jumping state.
         *
         * @param entity Pointer to the enemy entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Jump").
         * @param renderer The renderer for drawing textures.
         */
        EnemyJumpState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer) {
        }

        /**
         * @brief Executes the jumping state logic (e.g., updating the jump animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~EnemyJumpState() {}
    };

    /**
     * @class EnemyAttackState
     * @brief Represents the attacking state of an enemy.
     *
     * In this state, the enemy is performing an attack (e.g., swinging a weapon).
     */
    class EnemyAttackState : public State {
        bool ended;  ///< Boolean to track if the attack has ended.
        bool cooldown;  ///< Boolean to track if the attack is on cooldown.
    public:
        /**
         * @brief Constructor for initializing the enemy's attacking state.
         *
         * @param entity Pointer to the enemy entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Attack").
         * @param renderer The renderer for drawing textures.
         */
        EnemyAttackState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer), ended{ false }, cooldown{ false } {
        }

        /**
         * @brief Resets the attack state (e.g., sets the cooldown back).
         */
        virtual void reset() override;

        /**
         * @brief Executes the attack state logic (e.g., updating the attack animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~EnemyAttackState() {}
    };

    /**
     * @class EnemyHurtState
     * @brief Represents the hurt state of an enemy.
     *
     * In this state, the enemy is injured and may be in a brief stunned animation.
     */
    class EnemyHurtState : public State {
        unsigned int hurtTimer;  ///< Timer for controlling the duration of the hurt state.
    public:
        /**
         * @brief Constructor for initializing the enemy's hurt state.
         *
         * @param entity Pointer to the enemy entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Hurt").
         * @param renderer The renderer for drawing textures.
         */
        EnemyHurtState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer), hurtTimer{ 0 } {
        }

        /**
         * @brief Resets the hurt state (e.g., reset the hurt timer).
         */
        virtual void reset() override;

        /**
         * @brief Executes the hurt state logic (e.g., updating the hurt animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~EnemyHurtState() {}
    };

    /**
     * @class EnemyDeathState
     * @brief Represents the death state of an enemy.
     *
     * In this state, the enemy is dead and may play a death animation or trigger game over.
     */
    class EnemyDeathState : public State {
    public:
        /**
         * @brief Constructor for initializing the enemy's death state.
         *
         * @param entity Pointer to the enemy entity.
         * @param _w The width of the sprite.
         * @param _h The height of the sprite.
         * @param data The state data containing the animation details.
         * @param type The type of the state ("Death").
         * @param renderer The renderer for drawing textures.
         */
        EnemyDeathState(Entity* entity, int _w, int _h, StateData* data, string type, SDL_Renderer* renderer)
            : State(entity, _w, _h, data, type, renderer) {
        }

        /**
         * @brief Executes the death state logic (e.g., updating the death animation).
         *
         * @param delay The delay between frames.
         */
        virtual void act(Uint32 delay) override;

        /**
         * @brief Destructor for cleaning up resources.
         */
        virtual ~EnemyDeathState() {}
    };

}
