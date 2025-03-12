#pragma once

namespace myGame {

    /**
     * @enum StateResult
     * @brief Enum describing the result of the game's state.
     *
     * This enum is used to represent the current outcome of a game state.
     */
    enum class StateResult {
        /**
         * @brief The game state has ended.
         */
        ENDED,

        /**
         * @brief The game is still ongoing.
         */
        CONTINUES,

        /**
         * @brief The game ended in failure.
         */
        FAILED
    };

    /**
     * @enum States
     * @brief Enum describing the possible states of a player in the game.
     *
     * This enum is used to manage different player states in the game such as idle, running, jumping, etc.
     */
    enum class States {
        /**
         * @brief Idle state.
         */
        Idle,

        /**
         * @brief Running state.
         */
        Run,

        /**
         * @brief Jumping state.
         */
        Jump,

        /**
         * @brief Attacking state.
         */
        Attack,

        /**
         * @brief Hurt state (when the player takes damage).
         */
        Hurt,

        /**
         * @brief Death state.
         */
        Death,

        /**
         * @brief The count of states.
         */
        Count
    };

    /**
     * @enum Direction
     * @brief Enum describing the possible directions of movement.
     *
     * This enum is used to specify the direction of movement for the player or other objects in the game.
     */
    enum class Direction {
        /**
         * @brief No direction.
         */
        None,

        /**
         * @brief Up direction.
         */
        Up,

        /**
         * @brief Down direction.
         */
        Down,

        /**
         * @brief Right direction.
         */
        Right,

        /**
         * @brief Left direction.
         */
        Left,

        /**
         * @brief The count of directions.
         */
        Count
    };

    /**
     * @enum CollideType
     * @brief Enum describing the types of collisions.
     *
     * This enum is used to define different types of collisions for collision detection.
     */
    enum class CollideType {
        /**
         * @brief Right triangle type.
         */
        RightTriangle,

        /**
         * @brief Left triangle type.
         */
        LeftTriangle,

        /**
         * @brief Rectangle type.
         */
        Rectangle
    };

    /**
     * @enum TexturesID
     * @brief Enum representing the texture IDs used in the game.
     *
     * This enum is used to identify various textures that are applied to game objects.
     */
    enum class TexturesID {
        /**
         * @brief The player's texture.
         */
        Player,

        /**
         * @brief The enemy's texture.
         */
        Enemy,

        /**
         * @brief The tile texture.
         */
        Tile,
    };

    /**
     * @enum StatusCode
     * @brief Enum representing the correctness status.
     *
     * This enum is used to represent the correctness status of some operations or answers.
     */
    enum class StatusCode {
        /**
         * @brief The answer or operation is correct.
         */
        CORRECT,

        /**
         * @brief The answer or operation is incorrect.
         */
        UNCORRECT,
    };

    /**
     * @enum MapStatus
     * @brief Enum describing the status of the map.
     *
     * This enum is used to manage the status of a map in the game, like whether the map is active, won, or lost.
     */
    enum class MapStatus {
        /**
         * @brief The map is active.
         */
        Act,

        /**
         * @brief The map has been won.
         */
        Win,

        /**
         * @brief The map has been lost.
         */
        Lose,
    };

}
