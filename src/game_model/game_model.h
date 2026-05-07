#ifndef GAME_MODEL_H
#define GAME_MODEL_H

/**
 * @brief Public read-only interface to the Tetris game state.
 *
 * This module defines data structures exposed to the frontend
 * and provides accessors for the current game snapshot.
 *
 * The frontend must treat all returned data as immutable.
 */

#include <stdbool.h>
#include <sys/time.h>

#include "../../shared/game_config.h"

/**
 * @brief User input event representation.
 *
 * Encapsulates keyboard input passed from CLI to game logic.
 */
typedef struct {
  int key;
  bool hold;
} UserInput_t;

/**
 * @brief Finite set of game states used by the state machine.
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
 * @brief Snapshot of current game state for rendering.
 *
 * Contains all data required by the frontend.
 * Memory ownership remains with the backend.
 *
 * @warning player string is dynamically allocated and managed internally.
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
 * @brief Returns a snapshot of the current game state.
 */
GameInfo_t getCurrentGameInfo(void);

/**
 * @brief Returns current state of the game state machine.
 */
GameState_t getCurrentState(void);

#endif  // GAME_MODEL_H