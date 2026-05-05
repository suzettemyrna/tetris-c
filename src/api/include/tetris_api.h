#ifndef TETRIS_API_H
#define TETRIS_API_H

/**
 * @file tetris_api.h
 * @brief Public API for the Tetris game engine.
 *
 * This module provides an interface between the frontend (CLI or other UI)
 * and the internal game logic. It exposes the game loop entry point and
 * read-only access to the current game state.
 */

#include <stdbool.h>
#include <sys/time.h>

#include "../../shared/game_config.h"

/**
 * @brief Represents a user input event.
 *
 * @param key  Key code (implementation-defined, e.g. ncurses key)
 * @param hold Indicates whether the key is being held down
 */
typedef struct {
  int key;
  bool hold;
} UserInput_t;

/**
 * @brief Enumeration of all possible game states.
 */
typedef enum {
  STATE_START,     /**< Initial screen, waiting for player input */
  STATE_PAUSE,     /**< Game is paused */
  STATE_GAME_OVER, /**< Game over state */
  STATE_SPAWN,     /**< Spawning a new tetromino */
  STATE_MOVE,      /**< Active movement phase */
  STATE_SHIFT,     /**< Continuous horizontal movement (key hold) */
  STATE_MERGE      /**< Merging tetromino with the field */
} GameState_t;

/**
 * @brief Represents the current state of the state machine.
 *
 * This structure is intentionally minimal and exposed for read-only purposes.
 */
typedef struct {
  GameState_t state; /**< Current game state */
} StateMachine;

/**
 * @brief Snapshot of the current game state.
 *
 * This structure is returned by the backend and consumed by the frontend.
 * It contains all data required for rendering and displaying game status.
 *
 * @note All pointers are owned by the backend. The caller must NOT free them.
 */
typedef struct {
  int **field;    /**< Game field matrix (FIELD_HEIGHT x FIELD_WIDTH) */
  int **next;     /**< Next tetromino preview matrix (4x4) */
  int score;      /**< Current score */
  int high_score; /**< Highest recorded score */
  int level;      /**< Current difficulty level */
  int speed;      /**< Current game speed (implementation-defined) */
  char *player;   /**< Player name (null-terminated string) */
} GameInfo_t;

/**
 * @brief Starts and runs the main game loop.
 *
 * This function blocks until the game is terminated.
 * It handles input processing, state updates, and rendering coordination.
 */
void tetrisGameLoop(void);

/**
 * @brief Returns the current game state snapshot.
 *
 * @return Current GameInfo_t structure
 *
 * @note Returned data is managed internally and must not be modified.
 */
GameInfo_t getCurrentGameInfo(void);

/**
 * @brief Returns the current state of the game state machine.
 *
 * @return Current GameState_t value
 */
GameState_t getCurrentState(void);

#endif  // TETRIS_API_H